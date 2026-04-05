// ═══════════════════════════════════════════════════════════
// 🌿 AQUA TERRA — Node.js Express Server
// Smart Water & Plant Monitoring System Backend
// ═══════════════════════════════════════════════════════════

require('dotenv').config();
const express = require('express');
const cors = require('cors');
const path = require('path');
const db = require('./database');

// ---------------------------------------------------------------------------
// Configuration
// ---------------------------------------------------------------------------
const PORT = process.env.PORT || 3000;
const SERIAL_BAUD = parseInt(process.env.SERIAL_BAUD || '9600', 10);
const SERIAL_PORT_PATH = process.env.SERIAL_PORT || ''; // e.g. COM3 or /dev/ttyUSB0

// ---------------------------------------------------------------------------
// Initialize Database
// ---------------------------------------------------------------------------
db.init();

// ---------------------------------------------------------------------------
// Express App Setup
// ---------------------------------------------------------------------------
const app = express();
app.use(cors());
app.use(express.json());

// Serve the frontend (index.html) from the project root
app.use(express.static(path.join(__dirname)));

// ═══════════════════════════════════════════════════════════
// 🔌 SERIAL PORT — Arduino USB Communication (Server-Side)
// ═══════════════════════════════════════════════════════════

let serialConnected = false;
let latestSerialData = null;
let serialPort = null;

async function initSerial() {
  if (!SERIAL_PORT_PATH) {
    console.log('⚠️  No SERIAL_PORT set in .env — serial reading disabled.');
    console.log('   The frontend can still use Web Serial API directly.');
    console.log('   Or set SERIAL_PORT=COM3 (Windows) or /dev/ttyUSB0 (Linux)');
    return;
  }

  try {
    const { SerialPort, ReadlineParser } = require('serialport');
    serialPort = new SerialPort({
      path: SERIAL_PORT_PATH,
      baudRate: SERIAL_BAUD,
    });

    const parser = serialPort.pipe(new ReadlineParser({ delimiter: '\n' }));

    serialPort.on('open', () => {
      console.log(`✅ Serial port ${SERIAL_PORT_PATH} opened at ${SERIAL_BAUD} baud`);
      serialConnected = true;
    });

    // Debounce: process at most 1 reading per second
    let lastSaveTime = 0;

    parser.on('data', (line) => {
      try {
        const data = JSON.parse(line.trim());
        latestSerialData = data;

        const now = Date.now();
        if (now - lastSaveTime >= 1000) {
          lastSaveTime = now;
          db.save(data);
        }
      } catch (err) {
        // Non-JSON line from Arduino — ignore
      }
    });

    serialPort.on('error', (err) => {
      console.error('❌ Serial port error:', err.message);
      serialConnected = false;
    });

    serialPort.on('close', () => {
      console.log('🔌 Serial port closed');
      serialConnected = false;
    });
  } catch (err) {
    console.error('❌ Could not initialize serial port:', err.message);
    console.log('   Install serialport: npm install serialport');
  }
}

initSerial();

// ═══════════════════════════════════════════════════════════
// 📡 REST API ENDPOINTS
// ═══════════════════════════════════════════════════════════

// ---------------------------------------------------------------------------
// GET /api/latest — Most recent sensor reading
// ---------------------------------------------------------------------------
app.get('/api/latest', (req, res) => {
  try {
    // Prefer live serial data, fall back to DB
    if (latestSerialData) {
      return res.json({ success: true, data: latestSerialData, source: 'serial' });
    }
    const row = db.getLatest();
    if (row) {
      return res.json({ success: true, data: row, source: 'database' });
    }
    res.json({ success: true, data: null, source: 'none' });
  } catch (err) {
    res.status(500).json({ success: false, error: err.message });
  }
});

// ---------------------------------------------------------------------------
// GET /api/history — Historical readings
// Query params: ?limit=100 or ?hours=24
// ---------------------------------------------------------------------------
app.get('/api/history', (req, res) => {
  try {
    const limit = parseInt(req.query.limit || '100', 10);
    const hours = req.query.hours ? parseFloat(req.query.hours) : null;
    const rows = db.getHistory(limit, hours);
    res.json({ success: true, data: rows, count: rows.length });
  } catch (err) {
    res.status(500).json({ success: false, error: err.message });
  }
});

// ---------------------------------------------------------------------------
// POST /api/save — Save a new sensor reading
// Body: { humidity, temperature, soil, waterLevel, motorRunning }
// ---------------------------------------------------------------------------
app.post('/api/save', (req, res) => {
  try {
    const result = db.save(req.body);
    res.json({ success: true, id: result.id });
  } catch (err) {
    res.status(500).json({ success: false, error: err.message });
  }
});

// ---------------------------------------------------------------------------
// GET /api/stats — Min / Max / Avg statistics
// Query params: ?hours=24
// ---------------------------------------------------------------------------
app.get('/api/stats', (req, res) => {
  try {
    const hours = parseInt(req.query.hours || '24', 10);
    const stats = db.getStats(hours);
    res.json({ success: true, data: stats });
  } catch (err) {
    res.status(500).json({ success: false, error: err.message });
  }
});

// ---------------------------------------------------------------------------
// GET /api/status — Server & serial connection status
// ---------------------------------------------------------------------------
app.get('/api/status', (req, res) => {
  res.json({
    success: true,
    serial: {
      connected: serialConnected,
      port: SERIAL_PORT_PATH || 'not configured',
      baud: SERIAL_BAUD,
    },
    uptime: process.uptime(),
  });
});

// ---------------------------------------------------------------------------
// GET /api/export — Export readings as CSV
// ---------------------------------------------------------------------------
app.get('/api/export', (req, res) => {
  try {
    const hours = parseInt(req.query.hours || '168', 10); // default 7 days
    const rows = db.getHistory(10000, hours);

    const header = 'id,humidity,temperature,soil_moisture,water_level,motor_running,timestamp\n';
    const csvRows = rows.map(r =>
      `${r.id},${r.humidity},${r.temperature},${r.soil_moisture},${r.water_level},${r.motor_running},${r.timestamp}`
    ).join('\n');

    res.setHeader('Content-Type', 'text/csv');
    res.setHeader('Content-Disposition', 'attachment; filename=aqua_terra_export.csv');
    res.send(header + csvRows);
  } catch (err) {
    res.status(500).json({ success: false, error: err.message });
  }
});

// ---------------------------------------------------------------------------
// Fallback: serve index.html for any non-API route
// ---------------------------------------------------------------------------
app.get('*', (req, res) => {
  res.sendFile(path.join(__dirname, 'index.html'));
});

// ═══════════════════════════════════════════════════════════
// 🚀 START SERVER
// ═══════════════════════════════════════════════════════════
app.listen(PORT, () => {
  console.log('');
  console.log('═══════════════════════════════════════════════════');
  console.log('  🌿 AQUA TERRA — Smart Plant Monitor');
  console.log(`  🌐 Dashboard:  http://localhost:${PORT}`);
  console.log(`  📡 API:        http://localhost:${PORT}/api/latest`);
  console.log(`  🔌 Serial:     ${SERIAL_PORT_PATH || 'Not configured'}`);
  console.log('═══════════════════════════════════════════════════');
  console.log('');
});

// ═══════════════════════════════════════════════════════════
// 🔒 GRACEFUL SHUTDOWN
// ═══════════════════════════════════════════════════════════
process.on('SIGINT', () => {
  console.log('\n🛑 Shutting down gracefully...');
  if (serialPort && serialPort.isOpen) serialPort.close();
  db.close();
  process.exit(0);
});

process.on('SIGTERM', () => {
  if (serialPort && serialPort.isOpen) serialPort.close();
  db.close();
  process.exit(0);
});
