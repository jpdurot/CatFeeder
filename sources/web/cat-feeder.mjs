import {SettingsProvider} from './settings';
import i2cbus from 'i2c';
import {Scheduler} from './scheduler';

export class CatFeederFactory {
    static create() {
        if (!process.CatFeeder) {
            process.CatFeeder = new CatFeeder();
        }
        return process.CatFeeder;
    }
}

class CatFeeder {
    
    constructor() {
        this.settings = new SettingsProvider().get();
        this.i2caddress = 0x20;
        this.i2c = new i2cbus(this.i2caddress, {device: '/dev/i2c-1'});
        this.lastInfos = {};
        this.lastFeed = null;
        this.serviceIntervalHandle = null;
        this.maxRegister = 7;
        this.scheduler = new Scheduler();
        this.schedules = [];
    }
    
    // Public methods
    
    feed(value) {
        var feedAsync = (resolve, reject) => {
            
            this.getRegister(0x00)
            .then(command => {
                if (command > 0) {
                    console.log('Feeder is busy');
                    throw Error('Feeder is busy');
                } else {
                    console.log('Set food register...');
                    return this.setRegister(0x07, value);
                }
            })
            .then(() => this.setRegister(0x00, 0x03))
            .then(resolve)
            .catch(reject);
        };
        if (value <0 || value > 40) {
                return Promise.reject('Wrong food value');
        }
        return new Promise(feedAsync);
        
    }
    
    getCalibration() {
        return this.getRegister(0x02);
    }
    
    getCommand() {
        return this.getRegister(0x00);
    }
    
    setCalibration(value) {
        var setCalibrationAsync = (resolve, reject) => {
            if (value < 0 || value > 255) {
                reject('Wrong calibration value');
            } else {
                this.settings.calibration = value;
                this.setRegister(0x02, value).then(resolve, reject);
            }
        };
        return new Promise(setCalibrationAsync);
        
    }
    
    getInfos() {
        return Promise.resolve({infos: this.lastInfos, lastFeed: this.lastFeed});
    }
    
    getInfos_old() {
        var getInfosAsync = (resolve, reject) => {
            this.i2c.writeByte(0x00, (err) => {
                if (err) {
                    reject(err);
                    return;
                }
                this.i2c.read(this.maxRegister + 1, (err, data) => { 
                    if (err) {
                       reject(err);
                    } else {
                        this.lastInfos = {
                           command: data[0],
                            status: data[1],
                            calibration : data[2],
                            weight: data[3] * 256* 256 * 256 + data[4] * 256* 256 + data[5] * 256 + data[6],
                            requestedFood: data[7],
                       };
                       // TODO  : Copy object before returning
                       resolve(this.lastInfos);
                    }
                });
            });
        };
        return new Promise(getInfosAsync);
    }
    
    
    
    getLastFeed() {
        return Promise.resolve(this.lastFeed ? this.lastFeed : {} );
    }
    
    initFromSettings() {
        this.setCalibration(this.settings.calibration);
    }
    
    setSchedules(sched) {
        // TODO Check minute, hour and quantity values
        this.scheduler.clearJobs();
        for (var i = 0; i < sched.length; i++) {
            const quantity = sched[i].quantity;
            this.scheduler.addJob(
                {
                    hour: sched[i].hour,
                    minute: sched[i].minute,
                }, () => console.log("Must feed " + quantity + "g")
            );
        }
        this.schedules = sched;
        return Promise.resolve();
    }
    
    getSchedules() {
        return this.schedules;
    }
    
    getNextSchedule() {
        return this.scheduler.getNextJobDate();
    }
    
    startService() {
        this.settings.loadSettings();
        
        this.initFromSettings();
        if (!this.serviceIntervalHandle) {
            this.serviceIntervalHandle = setInterval(() => this.serviceLoop(), 1000);
        }
    }
    
    stopService() {
        if (this.serviceIntervalHandle) {
            clearInterval(this.serviceIntervalHandle);
        }
    }
    
    // Private methods (at least, should be...)
    
    setRegister(registerAddress, registerValue) {
        var setRegisterAsync = (resolve, reject) => {
            this.i2c.write([registerAddress, registerValue], (err) => {
                if (err) {
                    reject(err);
                    return;
                }
                resolve();
            });
        }
        return new Promise(setRegisterAsync);
    }
    
    getRegister(registerAddress) {
        var getRegisterAsync = (resolve, reject) => {
            this.i2c.writeByte(registerAddress, (err) => {
                if (err) {
                    reject(err);
                    return;
                }
                this.i2c.readByte((err, data) => { 
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
    
    getAllRegisters()  {
        var getAllRegistersAsync = (resolve, reject) => {
            var newInfos = {};
            this.getRegister(0x00)
                .then(data => {newInfos.command = data; return this.getRegister(0x01);})
                .then(data => {newInfos.status = data; return this.getRegister(0x02);})
                .then(data => {newInfos.calibration = data; return this.getRegister(0x03);})
                .then(data => {newInfos.weight = data * 256 * 256 * 256; return this.getRegister(0x04);})
                .then(data => {newInfos.weight += data * 256 * 256; return this.getRegister(0x05);})
                .then(data => {newInfos.weight += data * 256; return this.getRegister(0x06);})
                .then(data => {newInfos.weight += data; return this.getRegister(0x07);})
                .then(data => {newInfos.food = data; return newInfos; })
                .then(resolve)
                .catch(reject);
                
        };
        return new Promise(getAllRegistersAsync);
    }
    
    serviceLoop() {
        this.getAllRegisters().then(
            infos => {
                if (typeof this.lastInfos.command !==  'undefined' && (this.lastInfos.command !== infos.command)) {
                    this.onCommandChanged(this.lastInfos, infos);
                }
                this.lastInfos = infos;
            });
    }
    
    onCommandChanged(oldInfos, newInfos) {
        if (oldInfos.command === 3 && newInfos.command === 0) {
            console.log("Feeding is over...");
            
            this.lastFeed = {
                date: new Date().getTime(),
                quantity: newInfos.weight
            }
            console.log(this.lastFeed);
        } else if (oldInfos.command === 0 && newInfos.command === 3) {
            console.log("Feeding is starting...");
        }
    }
    
    
}


