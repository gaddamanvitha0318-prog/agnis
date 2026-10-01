/* AGNIS-4WD DRIVE JOYSTICK INTERFACE */
const joyZone = document.getElementById('joy-zone');
const joyKnob = document.getElementById('joy-knob');
const speedIndicator = document.getElementById('speed-indicator');

let isDragging = false;
let startX = 0, startY = 0;
let maxRadius = 80;
let lastSend = 0;

if (joyZone && joyKnob) {
    joyZone.addEventListener('pointerdown', (e) => {
        isDragging = true;
        const rect = joyZone.getBoundingClientRect();
        startX = rect.left + rect.width / 2;
        startY = rect.top + rect.height / 2;
        moveKnob(e.clientX, e.clientY);
    });

    window.addEventListener('pointermove', (e) => {
        if (!isDragging) return;
        moveKnob(e.clientX, e.clientY);
    });

    window.addEventListener('pointerup', () => {
        if (!isDragging) return;
        isDragging = false;
        joyKnob.style.transform = `translate(0px, 0px)`;
        sendDriveCommand(0, 0);
        if (speedIndicator) speedIndicator.innerText = `SPEED: 0%`;
    });
}

function moveKnob(clientX, clientY) {
    let dx = clientX - startX;
    let dy = clientY - startY;
    let distance = Math.sqrt(dx * dx + dy * dy);

    if (distance > maxRadius) {
        dx = (dx / distance) * maxRadius;
        dy = (dy / distance) * maxRadius;
    }

    joyKnob.style.transform = `translate(${dx}px, ${dy}px)`;

    // Convert dy -> Forward Speed (-255 to +255), dx -> Turn (-255 to +255)
    let speed = Math.round((-dy / maxRadius) * 255);
    let turn = Math.round((dx / maxRadius) * 255);

    let speedPct = Math.round((Math.abs(speed) / 255) * 100);
    if (speedIndicator) speedIndicator.innerText = `SPEED: ${speedPct}%`;

    const now = Date.now();
    if (now - lastSend > 50) { // 20Hz drive command rate
        lastSend = now;
        sendDriveCommand(speed, turn);
    }
}

function sendDriveCommand(speed, turn) {
    sendWsCommand({
        type: 'drive',
        speed: speed,
        turn: turn
    });
}

function setMode(modeStr) {
    document.querySelectorAll('.mode-btn').forEach(b => b.classList.remove('active'));
    if (modeStr === 'MANUAL') document.getElementById('btn-manual').classList.add('active');
    if (modeStr === 'ASSIST') document.getElementById('btn-assist').classList.add('active');
    if (modeStr === 'AUTO') document.getElementById('btn-auto').classList.add('active');

    sendWsCommand({
        type: 'mode',
        mode: modeStr
    });
}

function triggerEstop() {
    sendWsCommand({ type: 'estop' });
}

onTelemetry(data => {
    const vDf = document.getElementById('val-df');
    const vDr = document.getElementById('val-dr');
    const vV = document.getElementById('val-v');
    const badge = document.getElementById('safety-badge');

    if (vDf) vDf.innerHTML = `${data.df.toFixed(1)} <span class="metric-unit">cm</span>`;
    if (vDr) vDr.innerHTML = `${data.dr.toFixed(1)} <span class="metric-unit">cm</span>`;
    if (vV) vV.innerHTML = `${data.v.toFixed(2)} <span class="metric-unit">V</span>`;

    if (badge) {
        if (data.estop) {
            badge.innerText = 'EMERGENCY STOP';
            badge.className = 'status-badge emergency';
        } else {
            const modeNames = ['MANUAL', 'ASSIST', 'AUTO'];
            badge.innerText = modeNames[data.mode] || 'MANUAL';
            badge.className = data.mode === 0 ? 'status-badge' : 'status-badge warning';
        }
    }
});
