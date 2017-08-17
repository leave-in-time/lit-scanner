const electron = require('electron');
const app = electron.app;
const BrowserWindow = electron.BrowserWindow;
const ipcMain = require('electron').ipcMain;

const path = require('path');
const url = require('url');
const dgram = require('dgram');

let mainWindow;

const createWindow = () => {
	mainWindow = new BrowserWindow({ width: 800, height: 400 });
	mainWindow.loadURL(
		url.format({
			pathname: path.join(__dirname, 'public', 'index.html'),
			protocol: 'file:',
			slashes: true,
		})
	);
	// mainWindow.webContents.openDevTools();
	mainWindow.on('closed', () => {
		mainWindow = null;
	});
};

app.on('ready', createWindow);
app.on('window-all-closed', () => {
	if (process.platform !== 'darwin') app.quit();
});

app.on('activate', () => {
	if (mainWindow === null) createWindow();
});

// messaging stuff
const client = dgram.createSocket('udp4');
const host = '192.168.2.37';
const port = 8888;

const sendText = text => {
	const message = new Buffer(text, 'binary');
	client.send(message, 0, message.length, port, host);
};

ipcMain.on('text', (event, arg) => {
	sendText(arg || ' ');
});
