/* AGNIS-4WD TELEMETRY DASHBOARD HANDLER */
onTelemetry(data => {
    const tV = document.getElementById('t-v');
    const tI = document.getElementById('t-i');
    const tP = document.getElementById('t-p');
    const tSoc = document.getElementById('t-soc');

    if (tV) tV.innerHTML = `${data.v.toFixed(2)} <span class="metric-unit">V</span>`;
    if (tI) tI.innerHTML = `${data.i.toFixed(2)} <span class="metric-unit">A</span>`;
    if (tP) tP.innerHTML = `${data.p.toFixed(1)} <span class="metric-unit">W</span>`;
    if (tSoc) tSoc.innerHTML = `${Math.round(data.soc)} <span class="metric-unit">%</span>`;

    const tDf = document.getElementById('t-df');
    const tDr = document.getElementById('t-dr');
    const tRoll = document.getElementById('t-roll');
    const tPitch = document.getElementById('t-pitch');

    if (tDf) tDf.innerHTML = `${data.df.toFixed(1)} <span class="metric-unit">cm</span>`;
    if (tDr) tDr.innerHTML = `${data.dr.toFixed(1)} <span class="metric-unit">cm</span>`;
    if (tRoll) tRoll.innerHTML = `${data.roll.toFixed(1)} <span class="metric-unit">°</span>`;
    if (tPitch) tPitch.innerHTML = `${data.pitch.toFixed(1)} <span class="metric-unit">°</span>`;

    const tGpsFix = document.getElementById('t-gpsfix');
    const tSats = document.getElementById('t-sats');
    const tLat = document.getElementById('t-lat');
    const tLng = document.getElementById('t-lng');

    if (tGpsFix) {
        tGpsFix.innerText = data.gps_fix ? '3D FIX' : 'NO FIX';
        tGpsFix.style.color = data.gps_fix ? 'var(--status-green)' : 'var(--status-red)';
    }
    if (tSats) tSats.innerText = data.sats || 0;
    if (tLat) tLat.innerText = data.lat ? data.lat.toFixed(5) : '0.00000';
    if (tLng) tLng.innerText = data.lng ? data.lng.toFixed(5) : '0.00000';
});
