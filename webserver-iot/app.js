// call the packages we need
var ttn = require('ttn');  // the things network 

var region = 'eu';
var appId = 'atmega328_rfm95';
var accessKey = 'ttn-account-v2.ajLN31-9LCErcWEUUoemSvRA40ZF0FNLKcpK04kDS4U';

var client = new ttn.Client(region, appId, accessKey);

client.on('connect', function(connack) {
  console.log('[DEBUG]', 'Connect:', connack);
});

client.on('error', function(err) {
  console.error('[ERROR]', err.message);
});

client.on('activation', function(deviceId, data) {
  console.log('[INFO] ', 'Activation:', deviceId, JSON.stringify(data, null, 2));
});

client.on('device', null, 'down/scheduled', function(deviceId, data) {
  console.log('[INFO] ', 'Scheduled:', deviceId, JSON.stringify(data, null, 2));
});

client.on('message', function(deviceId, data) {
  console.info('[INFO] ', 'Message:', deviceId, JSON.stringify(data, null, 2));
});

client.on('message', null, 'led', function(deviceId, led) {
  // Toggle the LED
  var payload = {
    led: !led
  };

  // If you don't have an encoder payload function:
  // var payload = [led ? 0 : 1];
  console.log('[DEBUG]', 'Sending:', JSON.stringify(payload));
  client.send(deviceId, payload);
});