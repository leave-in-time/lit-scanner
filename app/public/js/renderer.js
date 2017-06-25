const ipcRenderer = require('electron').ipcRenderer;

const idle = document.querySelector('#idle');
const ko = document.querySelector('#ko');
const ok = document.querySelector('#ok');
const current = document.querySelector('#current');

idle.onclick = () => {
	ipcRenderer.send('text', '0');
	current.textContent = 'idle';
};

ko.onclick = () => {
	ipcRenderer.send('text', '1');
	current.textContent = 'ko';
};

ok.onclick = () => {
	ipcRenderer.send('text', '2');
	current.textContent = 'ok';
};
