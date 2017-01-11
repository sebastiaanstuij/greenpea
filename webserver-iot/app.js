// call the packages we need
var ttn = require('ttn');  // the things network 
request = require('request'); // for making http requests

var region = 'eu';
var appId = 'atmega328_rfm95';
var accessKey = 'ttn-account-v2.ajLN31-9LCErcWEUUoemSvRA40ZF0FNLKcpK04kDS4U';

var client = new ttn.Client(region, appId, accessKey);

client.on('connect', function(connack) {
  console.log('[DEBUG]', 'Connect:', connack);

  // temporary measure to send each log message via IFTT to email
  request.post({
			url: 'https://maker.ifttt.com/trigger/greenpea_mail/with/key/pWE3KbYZi6Z3Nh64cAtt1hf9SQsRzmUMUB7CUtPgxdy',
      payload: {
        'value1': 'type: connect',
        'value2': connack,
      }
		}, function(error, response, body) {
      if(error) {
			  console.log('Error was ', error);
      }
			console.log('Post successful, server responded with: ', body);
		});
});

client.on('error', function(err) {
  console.error('[ERROR]', err.message);

  // temporary measure to send each log message via IFTT to email
  request.post({
			url: 'https://maker.ifttt.com/trigger/greenpea_mail/with/key/pWE3KbYZi6Z3Nh64cAtt1hf9SQsRzmUMUB7CUtPgxdy',
      payload: {
        'value1': 'type: error',
        'value2': err,
      }
		}, function(error, response, body) {
      if(error) {
			  console.log('Error was ', error);
      }
			console.log('Post successful, server responded with: ', body);
		});

});

client.on('activation', function(deviceId, data) {
  console.log('[INFO] ', 'Activation:', deviceId, JSON.stringify(data, null, 2));

  var deviceString = 'deviceID: ' + deviceId;
  // temporary measure to send each log message via IFTT to email
  request.post({
      
			url: 'https://maker.ifttt.com/trigger/greenpea_mail/with/key/pWE3KbYZi6Z3Nh64cAtt1hf9SQsRzmUMUB7CUtPgxdy',
      payload: {
        'value1': deviceString,
        'value2': 'type: activation',
        'value3': data
      }
		}, function(error, response, body) {
      if(error) {
			  console.log('Error was ', error);
      }
			console.log('Post successful, server responded with: ', body);
		});
});

client.on('device', null, 'down/scheduled', function(deviceId, data) {
  console.log('[INFO] ', 'Scheduled:', deviceId, JSON.stringify(data, null, 2));

  var deviceString = 'deviceID: ' + deviceId;
  // temporary measure to send each log message via IFTT to email
  request.post({
      
			url: 'https://maker.ifttt.com/trigger/greenpea_mail/with/key/pWE3KbYZi6Z3Nh64cAtt1hf9SQsRzmUMUB7CUtPgxdy',
      payload: {
        'value1': deviceString,
        'value2': 'type: device',
        'value3': data
      }
		}, function(error, response, body) {
      if(error) {
			  console.log('Error was ', error);
      }
			console.log('Post successful, server responded with: ', body);
		});
});

client.on('message', function(deviceId, data) {
  console.info('[INFO] ', 'Message:', deviceId, JSON.stringify(data, null, 2));
  
  var deviceString = 'deviceID: ' + deviceId;
  // temporary measure to send each log message via IFTT to email
  request.post({
      
			url: 'https://maker.ifttt.com/trigger/greenpea_mail/with/key/pWE3KbYZi6Z3Nh64cAtt1hf9SQsRzmUMUB7CUtPgxdy',
      payload: {
        'value1': deviceString,
        'value2': 'type: message',
        'value3': data
      }
		}, function(error, response, body) {
      if(error) {
			  console.log('Error was ', error);
      }
			console.log('Post successful, server responded with: ', body);
		});

});

client.on('message', null, 'led', function(deviceId, led) {

  var deviceString = 'deviceID: ' + deviceId;
  // temporary measure to send each log message via IFTT to email
  request.post({
      
			url: 'https://maker.ifttt.com/trigger/greenpea_mail/with/key/pWE3KbYZi6Z3Nh64cAtt1hf9SQsRzmUMUB7CUtPgxdy',
      payload: {
        'value1': deviceString,
        'value2': 'type: message, led',
        'value3': led
      }
		}, function(error, response, body) {
      if(error) {
			  console.log('Error was ', error);
      }
			console.log('Post successful, server responded with: ', body);
		});

  // Toggle the LED
  var payload = {
    led: !led
  };

  // If you don't have an encoder payload function:
  // var payload = [led ? 0 : 1];

  console.log('[DEBUG]', 'Sending:', JSON.stringify(payload));
  client.send(deviceId, payload);
});