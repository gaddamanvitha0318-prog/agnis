/* AGNIS-4WD COMMON WEBSOCKET & REST CLIENT */
let socket = null;
let telemetryCallbacks = [];

function initWebSocket() {
    const wsHost = window.location.hostname || '192.168.4.1';
    socket = new WebSocket(`ws://${wsHost}/ws`);

    socket.onopen = function() {
        console.log('[WS] Connected to AGNIS-4WD server');
        const badge = document.getElementById('sys-status') || document.getElementById('telem-badge');
        if (badge) {
            badge.innerText = 'ONLINE';
            badge.className = 'status-badge';
        }
    };

    socket.onmessage = function(event) {
        try {
            const data = JSON.parse(event.data);
            telemetryCallbacks.forEach(cb => cb(data));
        } catch(e) {
            console.error('[WS] Parse error', e);
        }
    };

    socket.onclose = function() {
        console.log('[WS] Disconnected, reconnecting in 2s...');
        const badge = document.getElementById('sys-status') || document.getElementById('telem-badge');
        if (badge) {
            badge.innerText = 'OFFLINE';
            badge.className = 'status-badge emergency';
        }
        setTimeout(initWebSocket, 2000);
    };
}

function sendWsCommand(obj) {
    if (socket && socket.readyState === WebSocket.OPEN) {
        socket.send(JSON.stringify(obj));
    }
}

function onTelemetry(cb) {
    telemetryCallbacks.push(cb);
}

document.addEventListener('DOMContentLoaded', initWebSocket);
