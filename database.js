// ═══════════════════════════════════════════════════════════
// 📦 DATABASE MODULE — SQLite3 Initialization & Query Helpers
// ═══════════════════════════════════════════════════════════

const Database = require('better-sqlite3');
const path = require('path');
require('dotenv').config();

// ---------------------------------------------------------------------------
// Database path — configurable via .env or defaults to ./aqua_terra.db
// ---------------------------------------------------------------------------
const DB_PATH = process.env.DB_PATH || path.join(__dirname, 'aqua_terra.db');

let db;

// ═══════════════════════════════════════════════════════════
// 🔧 INITIALIZATION — Auto-creates table on first run
// ═══════════════════════════════════════════════════════════
function init() {
  db = new Database(DB_PATH);
  db.pragma('journal_mode = WAL'); // Better concurrent read performance

  db.exec(`
    CREATE TABLE IF NOT EXISTS sensor_readings (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      humidity REAL,
      temperature REAL,
      soil_moisture INTEGER,
      water_level INTEGER,
      motor_running INTEGER,
      timestamp DATETIME DEFAULT CURRENT_TIMESTAMP
    );
  `);

  const columns = db.prepare(`PRAGMA table_info(sensor_readings)`).all().map(row => row.name);
  if (!columns.includes('motor_running')) {
    db.exec(`ALTER TABLE sensor_readings ADD COLUMN motor_running INTEGER`);
  }

  // Create index on timestamp for faster history queries
  db.exec(`
    CREATE INDEX IF NOT EXISTS idx_timestamp ON sensor_readings(timestamp);
  `);

  console.log(`✅ Database initialized at ${DB_PATH}`);
  return db;
}

// ═══════════════════════════════════════════════════════════
// 💾 SAVE — Insert a new sensor reading
// ═══════════════════════════════════════════════════════════
const insertStmt = () => db.prepare(`
  INSERT INTO sensor_readings (humidity, temperature, soil_moisture, water_level, motor_running)
  VALUES (@humidity, @temperature, @soil_moisture, @water_level, @motor_running)
`);

let _insertStmt = null;

function save(reading) {
  if (!_insertStmt) _insertStmt = insertStmt();
  const info = _insertStmt.run({
    humidity: reading.humidity ?? null,
    temperature: reading.temperature ?? null,
    soil_moisture: reading.soil ?? reading.soil_moisture ?? null,
    water_level: reading.waterLevel ?? reading.water_level ?? null,
    motor_running: reading.motorRunning ?? reading.motor_running ?? reading.relay ?? null,
  });
  return { id: info.lastInsertRowid };
}

// ═══════════════════════════════════════════════════════════
// 📡 LATEST — Get the most recent sensor reading
// ═══════════════════════════════════════════════════════════
function getLatest() {
  return db.prepare(`
    SELECT * FROM sensor_readings ORDER BY id DESC LIMIT 1
  `).get() || null;
}

// ═══════════════════════════════════════════════════════════
// 📜 HISTORY — Get last N readings (default 100)
// ═══════════════════════════════════════════════════════════
function getHistory(limit = 100, hours = null) {
  if (hours) {
    return db.prepare(`
      SELECT * FROM sensor_readings
      WHERE timestamp >= datetime('now', '-' || ? || ' hours')
      ORDER BY timestamp ASC
    `).all(hours);
  }
  return db.prepare(`
    SELECT * FROM sensor_readings ORDER BY timestamp DESC LIMIT ?
  `).all(limit).reverse(); // reverse so oldest is first for charting
}

// ═══════════════════════════════════════════════════════════
// 📊 STATS — Min / Max / Avg for each sensor
// ═══════════════════════════════════════════════════════════
function getStats(hours = 24) {
  return db.prepare(`
    SELECT
      MIN(humidity)       AS humidity_min,
      MAX(humidity)       AS humidity_max,
      ROUND(AVG(humidity), 1) AS humidity_avg,
      MIN(temperature)    AS temperature_min,
      MAX(temperature)    AS temperature_max,
      ROUND(AVG(temperature), 1) AS temperature_avg,
      MIN(soil_moisture)  AS soil_min,
      MAX(soil_moisture)  AS soil_max,
      ROUND(AVG(soil_moisture), 0) AS soil_avg,
      MIN(water_level)    AS water_min,
      MAX(water_level)    AS water_max,
      ROUND(AVG(water_level), 0) AS water_avg,
      COUNT(*) AS total_readings
    FROM sensor_readings
    WHERE timestamp >= datetime('now', '-' || ? || ' hours')
  `).get(hours);
}

// ═══════════════════════════════════════════════════════════
// 🧹 CLEANUP — Delete readings older than N days
// ═══════════════════════════════════════════════════════════
function cleanup(days = 30) {
  const info = db.prepare(`
    DELETE FROM sensor_readings
    WHERE timestamp < datetime('now', '-' || ? || ' days')
  `).run(days);
  return { deleted: info.changes };
}

// ═══════════════════════════════════════════════════════════
// 🔒 CLOSE — Graceful shutdown
// ═══════════════════════════════════════════════════════════
function close() {
  if (db) {
    db.close();
    console.log('🔒 Database connection closed');
  }
}

module.exports = {
  init,
  save,
  getLatest,
  getHistory,
  getStats,
  cleanup,
  close,
};
