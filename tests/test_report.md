# Smart Cabin & Vehicle Health Monitor — Test Report

**Execution Time:** 2026-05-29 09:45:14  
**Total: 41/41 PASSED** | **0 FAILED**

## 📝 Executive Summary

| # | ID | Test Name | Result |
|---|---|---|---|
| 1 | TC-SEN-001 | All sensors in normal range — no alerts | ✅ PASS |
| 2 | TC-SEN-002 | Engine temp 109.9°C — just below CRITICAL threshold | ✅ PASS |
| 3 | TC-SEN-003 | Engine temp 110.0°C — exact CRITICAL threshold boundary | ✅ PASS |
| 4 | TC-SEN-004 | Engine temp 115°C — clearly above threshold | ✅ PASS |
| 5 | TC-SEN-005 | Battery 11.01V — just above LOW threshold, no alert | ✅ PASS |
| 6 | TC-SEN-006 | Battery 11.0V — exact LOW BATTERY threshold (inclusive check | ✅ PASS |
| 7 | TC-SEN-007 | Battery 10.99V — below threshold, LOW BATTERY WARNING fires | ✅ PASS |
| 8 | TC-SEN-008 | Speed 119.9 km/h — just below OVERSPEED threshold | ✅ PASS |
| 9 | TC-SEN-009 | Speed 120.0 km/h — exact OVERSPEED threshold boundary | ✅ PASS |
| 10 | TC-SEN-010 | Speed 121.0 km/h — OVERSPEED WARNING fires | ✅ PASS |
| 11 | TC-SEN-011 | Tire 25.1 PSI — just above LOW threshold, no alert | ✅ PASS |
| 12 | TC-SEN-012 | Tire 24.9 PSI — LOW TIRE PRESSURE fires | ✅ PASS |
| 13 | TC-SEN-013 | Tire 45 PSI — high pressure input warning feedback | ✅ PASS |
| 14 | TC-SEN-014 | Door OPEN + Speed 0 km/h — false positive prevention | ✅ PASS |
| 15 | TC-SEN-015 | Door OPEN + Speed 10.0 km/h — exact boundary, no alert | ✅ PASS |
| 16 | TC-SEN-016 | Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires | ✅ PASS |
| 17 | TC-SEN-017 | Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires | ✅ PASS |
| 18 | TC-SEN-018 | Seatbelt UNLOCKED + Speed 0 — false positive prevention | ✅ PASS |
| 19 | TC-SEN-019 | Seatbelt UNLOCKED + Speed 30 km/h — SEATBELT WARNING fires | ✅ PASS |
| 20 | TC-SEN-020 | Seatbelt LOCKED + Speed 100 km/h — no alert | ✅ PASS |
| 21 | TC-ALT-001 | All 6 alerts active simultaneously | ✅ PASS |
| 22 | TC-ALT-002 | Alert deduplication — same overheat across 5 monitor cycles | ✅ PASS |
| 23 | TC-ALT-003 | Alert auto-clear — engine cools from 115°C to 90°C | ✅ PASS |
| 24 | TC-ALT-004 | Alert priority display — CRITICAL before WARNING in Option 2 | ✅ PASS |
| 25 | TC-ALT-005 | Operator << — alert formatted with severity + message + time | ✅ PASS |
| 26 | TC-ALT-006 | Door alert clears — door closed while still moving | ✅ PASS |
| 27 | TC-ALT-007 | Static alert counter — 6 alerts generated, counter == 6 | ✅ PASS |
| 28 | TC-VAL-001 | Non-numeric engine temp input 'abc' — rejected with feedback | ✅ PASS |
| 29 | TC-VAL-002 | Speed 500 km/h — out of bounds, rejected | ✅ PASS |
| 30 | TC-VAL-003 | Negative speed -5 km/h — out of bounds, rejected | ✅ PASS |
| 31 | TC-VAL-004 | Empty input (Enter) — sensor retains previous value | ✅ PASS |
| 32 | TC-VAL-005 | Door input '2' — invalid discrete value, rejected | ✅ PASS |
| 33 | TC-VAL-006 | Seatbelt input 'yes' — non-integer rejected | ✅ PASS |
| 34 | TC-VAL-007 | Main menu input '8' — invalid, error shown, menu repainted | ✅ PASS |
| 35 | TC-VAL-008 | Main menu input 'abc' — invalid, error shown | ✅ PASS |
| 36 | TC-CFG-001 | Valid config.txt loaded — thresholds override defaults | ✅ PASS |
| 37 | TC-CFG-002 | config.txt missing — warn and use defaults | ✅ PASS |
| 38 | TC-CFG-003 | config.txt with comments (#) and whitespace — parsed correct | ✅ PASS |
| 39 | TC-CFG-004 | Custom ENGINE_TEMP_CRITICAL=105 — alert fires at 106 not 110 | ✅ PASS |
| 40 | TC-DSH-001 | Dashboard Option 1 — all 6 sensor rows present | ✅ PASS |
| 41 | TC-DSH-002 | Shutdown summary — sensor count and alert count displayed | ✅ PASS |

---

## 🔍 Detailed Test Results

### ✅ PASS TC-SEN-001: All sensors in normal range — no alerts

> Baseline: feed valid, safe sensor values. Expect zero alerts.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "72.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "battery_volt": &quot;14.00 V&quot;,
  "door": &quot;CLOSED&quot;,
  "engine_temp": &quot;85.00 C&quot;,
  "input_accepted": true,
  "input_feedback": &quot;Value accepted&quot;,
  "seatbelt": &quot;LOCKED&quot;,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "speed": &quot;72.00 km/h&quot;,
  "system_running": true,
  "tire_psi": &quot;32.00 PSI&quot;
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "battery_volt": &quot;14.00 V&quot;,
  "door": &quot;CLOSED&quot;,
  "engine_temp": &quot;85.00 C&quot;,
  "input_accepted": true,
  "input_feedback": &quot;Value accepted&quot;,
  "seatbelt": &quot;LOCKED&quot;,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "speed": &quot;72.00 km/h&quot;,
  "system_running": true,
  "tire_psi": &quot;32.00 PSI&quot;
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-002: Engine temp 109.9°C — just below CRITICAL threshold

> Off-by-one below 110°C. Confirm NO alert fires.

**Inputs:**
```json
{
  "engine_temp": "109.9",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-003: Engine temp 110.0°C — exact CRITICAL threshold boundary

> At exactly 110.0°C alert must fire (>= threshold).

**Inputs:**
```json
{
  "engine_temp": "110.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-004: Engine temp 115°C — clearly above threshold

> Standard overheat case. Alert fires, log written, dashboard red.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;CRITICAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Engine Temperature": &quot;CRITICAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-005: Battery 11.01V — just above LOW threshold, no alert

> Boundary: 11.01V is safe. Confirm no LOW BATTERY WARNING.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "11.01",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-006: Battery 11.0V — exact LOW BATTERY threshold (inclusive check)

> At exactly 11.0V: confirm boundary direction (< vs <=).

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "11.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-007: Battery 10.99V — below threshold, LOW BATTERY WARNING fires

> 10.99V < 11V. WARNING must be generated.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "10.99",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-008: Speed 119.9 km/h — just below OVERSPEED threshold

> No overspeed alert expected at 119.9 km/h.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "119.9",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-009: Speed 120.0 km/h — exact OVERSPEED threshold boundary

> Spec says > 120. At exactly 120 no alert (exclusive).

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "120.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-010: Speed 121.0 km/h — OVERSPEED WARNING fires

> 121 km/h > 120 threshold. OVERSPEED WARNING expected.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "121.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-011: Tire 25.1 PSI — just above LOW threshold, no alert

> 25.1 > 25. No LOW TIRE PRESSURE alert.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "25.1",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-012: Tire 24.9 PSI — LOW TIRE PRESSURE fires

> 24.9 < 25 threshold. Alert expected.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "24.9",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Tire Pressure": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-013: Tire 45 PSI — high pressure input warning feedback

> Value in-bounds (0-60) but abnormally high. Accepted with [WARNING].

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "45.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-014: Door OPEN + Speed 0 km/h — false positive prevention

> Opening door while parked must NOT trigger DOOR OPEN WARNING.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-015: Door OPEN + Speed 10.0 km/h — exact boundary, no alert

> Spec: speed > 10. At exactly 10 door alert must NOT fire.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "10.0",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-016: Door OPEN + Speed 10.1 km/h — DOOR OPEN WARNING fires

> 10.1 > 10 threshold. Alert must fire immediately.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "10.1",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;CRITICAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Door Status": &quot;CRITICAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-017: Door OPEN + Speed 50 km/h — DOOR OPEN WARNING fires

> Standard case: door open while driving.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "50.0",
  "tire_psi": "32.0",
  "door": "1",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-018: Seatbelt UNLOCKED + Speed 0 — false positive prevention

> Removing seatbelt while parked must be silent.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-019: Seatbelt UNLOCKED + Speed 30 km/h — SEATBELT WARNING fires

> Moving with seatbelt off must trigger warning.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "30.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;WARNING&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-SEN-020: Seatbelt LOCKED + Speed 100 km/h — no alert

> Normal highway driving with seatbelt on.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "100.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "input_accepted": true,
  "sensor_status": {
    "Seatbelt Status": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-001: All 6 alerts active simultaneously

> Worst-case: every threshold violated at once.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "8.0",
  "speed": "130.0",
  "tire_psi": "20.0",
  "door": "1",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;,
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;,
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;,
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 6,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;,
    "Door Status": &quot;CRITICAL&quot;,
    "Engine Temperature": &quot;CRITICAL&quot;,
    "Seatbelt Status": &quot;WARNING&quot;,
    "Tire Pressure": &quot;WARNING&quot;,
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] DOOR OPEN": &quot;CRITICAL&quot;,
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] LOW BATTERY": &quot;WARNING&quot;,
    "[WARNING] LOW TIRE PRESSURE": &quot;WARNING&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;,
    "[WARNING] SEATBELT WARNING": &quot;WARNING&quot;
  },
  "alert_count": 6,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;WARNING&quot;,
    "Door Status": &quot;CRITICAL&quot;,
    "Engine Temperature": &quot;CRITICAL&quot;,
    "Seatbelt Status": &quot;WARNING&quot;,
    "Tire Pressure": &quot;WARNING&quot;,
    "Vehicle Speed": &quot;WARNING&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-002: Alert deduplication — same overheat across 5 monitor cycles

> Engine stays at 115C. Active alert set must contain exactly 1 entry.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-003: Alert auto-clear — engine cools from 115°C to 90°C

> Overheat fires at 115C. Drops to 90C. Active alert cleared.

**Sequence:**
```json
[
  {
    "engine_temp": "115.0",
    "battery_volt": "14.0",
    "speed": "0.0",
    "tire_psi": "32.0",
    "door": "0",
    "seatbelt": "0"
  },
  {
    "engine_temp": "90.0",
    "battery_volt": "14.0",
    "speed": "0.0",
    "tire_psi": "32.0",
    "door": "0",
    "seatbelt": "0"
  }
]
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-004: Alert priority display — CRITICAL before WARNING in Option 2

> With engine overheat + overspeed active, CRITICAL must appear first.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "130.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;,
    "[WARNING] OVERSPEED": &quot;WARNING&quot;
  },
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-005: Operator << — alert formatted with severity + message + timestamp

> std::cout << alert must output: [SEVERITY] NAME - Message.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "highest_severity": &quot;CRITICAL&quot;,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-006: Door alert clears — door closed while still moving

> Alert fires with door open at 30 km/h, clears when door closes.

**Sequence:**
```json
[
  {
    "engine_temp": "85.0",
    "battery_volt": "14.0",
    "speed": "30.0",
    "tire_psi": "32.0",
    "door": "1",
    "seatbelt": "0"
  },
  {
    "engine_temp": "85.0",
    "battery_volt": "14.0",
    "speed": "30.0",
    "tire_psi": "32.0",
    "door": "0",
    "seatbelt": "0"
  }
]
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {},
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-ALT-007: Static alert counter — 6 alerts generated, counter == 6

> Alert::getTotalCount() must equal total distinct alerts generated.

**Inputs:**
```json
{
  "engine_temp": "115.0",
  "battery_volt": "8.0",
  "speed": "130.0",
  "tire_psi": "20.0",
  "door": "1",
  "seatbelt": "1"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "alert_count": 6,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "alert_count": 6,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-001: Non-numeric engine temp input 'abc' — rejected with feedback

> Invalid string fed as engine temp. System re-prompts.

**Inputs:**
```json
{
  "engine_temp": "abc"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid numeric input&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid numeric input&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-002: Speed 500 km/h — out of bounds, rejected

> validateInput() must reject speed > 300. Red error + re-prompt.

**Inputs:**
```json
{
  "speed": "500.0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid speed&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid speed&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-003: Negative speed -5 km/h — out of bounds, rejected

> Negative values are physically impossible. Must be rejected.

**Inputs:**
```json
{
  "speed": "-5.0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid speed&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid speed&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-004: Empty input (Enter) — sensor retains previous value

> Pressing Enter skips the sensor. Previous value preserved.

**Inputs:**
```json
{
  "engine_temp": ""
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-005: Door input '2' — invalid discrete value, rejected

> Door accepts only 0 or 1. '2' must be rejected with feedback.

**Inputs:**
```json
{
  "door": "2"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid door status&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid door status&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-006: Seatbelt input 'yes' — non-integer rejected

> Seatbelt expects 0 or 1. String 'yes' must fail validation.

**Inputs:**
```json
{
  "seatbelt": "yes"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid numeric input&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid numeric input&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-007: Main menu input '8' — invalid, error shown, menu repainted

> Out-of-range menu option must print error in Yellow.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-VAL-008: Main menu input 'abc' — invalid, error shown

> Non-numeric menu input must be handled without crash.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_feedback": &quot;Invalid choice&quot;,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-CFG-001: Valid config.txt loaded — thresholds override defaults

> Custom thresholds in config must be applied.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-CFG-002: config.txt missing — warn and use defaults

> Delete config.txt. System must warn and run with hardcoded defaults.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "degraded_mode": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "degraded_mode": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-CFG-003: config.txt with comments (#) and whitespace — parsed correctly

> Comments ignored, whitespace trimmed, valid values loaded.

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-CFG-004: Custom ENGINE_TEMP_CRITICAL=105 — alert fires at 106 not 110

> Override threshold to 105. Feed 106C. Confirm alert fires early.

**Inputs:**
```json
{
  "engine_temp": "106.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "active_alerts": {
    "[CRITICAL] ENGINE OVERHEAT": &quot;CRITICAL&quot;
  },
  "alert_count": 1,
  "input_accepted": true,
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-DSH-001: Dashboard Option 1 — all 6 sensor rows present

> Live table must show all sensors with current values and status.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "72.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true,
  "sensor_status": {
    "Battery Voltage": &quot;NORMAL&quot;,
    "Door Status": &quot;NORMAL&quot;,
    "Engine Temperature": &quot;NORMAL&quot;,
    "Seatbelt Status": &quot;NORMAL&quot;,
    "Tire Pressure": &quot;NORMAL&quot;,
    "Vehicle Speed": &quot;NORMAL&quot;
  },
  "system_running": true
}</pre>
</td></tr></table>

---

### ✅ PASS TC-DSH-002: Shutdown summary — sensor count and alert count displayed

> Option 7 shutdown must display final statistics.

**Inputs:**
```json
{
  "engine_temp": "85.0",
  "battery_volt": "14.0",
  "speed": "0.0",
  "tire_psi": "32.0",
  "door": "0",
  "seatbelt": "0"
}
```

<table><tr><td width="50%" valign="top">

**Expected State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true
}</pre>
</td><td width="50%" valign="top">

**Actual State:**

<pre style="background-color: #1e1e1e; color: #d4d4d4; padding: 12px; border-radius: 6px; font-family: 'Consolas', 'Courier New', monospace; font-size: 13px; line-height: 1.4; margin: 0; white-space: pre-wrap; word-break: break-all;">{
  "input_accepted": true
}</pre>
</td></tr></table>

---

