var CatFeeder = function () {
    var self = this;
    
    var i2cbus = require('i2c');
    var i2caddress = 0x20;

    var i2c = new i2cbus(i2caddress, {device: '/dev/i2c-1'});
    
    var lastInfos = {} ;
    var lastFeed = null;
    
    var serviceIntervalHandle;
    
    const maxRegister = 7;
    
    function setRegister(registerAddress, registerValue) {
        var setRegisterAsync = function(resolve, reject) {
            i2c.write([registerAddress, registerValue], function (err) {
                if (err) {
                    reject(err);
                    return;
                }
                resolve();
            });
        }
        return new Promise(setRegisterAsync);
    }
    function getRegister(registerAddress) {
        var getRegisterAsync = function(resolve, reject) {
            i2c.writeByte(registerAddress, function(err) {
                if (err) {
                    reject(err);
                    return;
                }
                i2c.readByte(function(err, data) { 
                    if (err) {
                       reject(err);
                    } else {
                        resolve(data);
                    }
                });
            });
        };
        return new Promise(getRegisterAsync);
    }
    
    function getAllRegisters()  {
        var getAllRegistersAsync = function(resolve, reject) {
            var newInfos = {};
            getRegister(0x00)
                .then(data => {newInfos.command = data; return getRegister(0x01);})
                .then(data => {newInfos.status = data; return getRegister(0x02);})
                .then(data => {newInfos.calibration = data; return getRegister(0x03);})
                .then(data => {newInfos.weight = data * 256 * 256 * 256; return getRegister(0x04);})
                .then(data => {newInfos.weight += data * 256 * 256; return getRegister(0x05);})
                .then(data => {newInfos.weight += data * 256; return getRegister(0x06);})
                .then(data => {newInfos.weight += data; return getRegister(0x07);})
                .then(data => {newInfos.food = data; return newInfos; })
                .then(resolve)
                .catch(reject);
                
        };
        return new Promise(getAllRegistersAsync);
    }
    
    function serviceLoop() {
        getAllRegisters().then(
            infos => {
                if (typeof lastInfos.command !==  'undefined' && (lastInfos.command !== infos.command)) {
                    onCommandChanged(lastInfos, infos);
                }
                lastInfos = infos;
            });
    }
    
    function onCommandChanged(oldInfos, newInfos) {
        if (oldInfos.command === 3 && newInfos.command === 0) {
            console.log("Feeding is over...");
            
            lastFeed = {
                date: new Date().getTime(),
                quantity: newInfos.weight
            }
            console.log(lastFeed);
        } else if (oldInfos.command === 0 && newInfos.command === 3) {
            console.log("Feeding is starting...");
        }
    }
    
    self.feed = function(value) {
        var feedAsync = function(resolve, reject) {
            
            getRegister(0x00)
            .then(command => {
                if (command > 0) {
                    console.log('Feeder is busy');
                    throw Error('Feeder is busy');
                } else {
                    console.log('Set food register...');
                    return setRegister(0x07, value);
                }
            })
            .then(() => setRegister(0x00, 0x03))
            .then(resolve)
            .catch(reject);
        };
        if (value <0 || value > 40) {
                return Promise.reject('Wrong food value');
        }
        return new Promise(feedAsync);
        
    }
    
    self.getCalibration = function() {
        return getRegister(0x02);
    }
    
    self.getCommand = function() {
        return getRegister(0x00);
    }
    
    self.setCalibration = function(value) {
        var setCalibrationAsync = function(resolve, reject) {
            if (value <0 || value > 255) {
                reject('Wrong calibration value');
            } else {
                setRegister(0x02, value).then(resolve, reject);
            }
        };
        return new Promise(setCalibrationAsync);
        
    }
    
    self.getInfos = function() {
        return Promise.resolve({infos: lastInfos, lastFeed: lastFeed});
    }
    
    self.getInfos_old = function () {
        var getInfosAsync = function(resolve, reject) {
            i2c.writeByte(0x00, function(err) {
                if (err) {
                    reject(err);
                    return;
                }
                i2c.read(maxRegister + 1, function(err, data) { 
                    if (err) {
                       reject(err);
                    } else {
                        console.dir(data);
                        self.lastInfos = {
                           command: data[0],
                            status: data[1],
                            calibration : data[2],
                            weight: data[3] * 256* 256 * 256 + data[4] * 256* 256 + data[5] * 256 + data[6],
                            requestedFood: data[7],
                       };
                       // TODO  : Copy object before returning
                       resolve(self.lastInfos);
                    }
                });
            });
        };
        return new Promise(getInfosAsync);
    }
    
    
    
    self.getLastFeed = function() {
        return Promise.resolve(lastFeed ? lastFeed : {} );
    }
    
    self.startService = function() {
        if (!serviceIntervalHandle) {
            serviceIntervalHandle = setInterval(serviceLoop, 1000);
        }
    }
    
    self.stopService = function() {
        if (serviceIntervalHandle) {
            clearInterval(serviceIntervalHandle);
        }
    }
    return self;
    
    


}

var catFeederFactory = {
    create: function() {
        if (!process.CatFeeder) {
            process.CatFeeder = new CatFeeder();
        }
        return process.CatFeeder;
    }
}



module.exports = catFeederFactory;
