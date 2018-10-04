import fs from 'fs';

export class SettingsProvider {
    
    constructor() {
        if (!process.settings) {
            process.settings = new Settings();
        }
    }
    get() {
        return process.settings;
    }
}

class Settings {

    constructor() {
        this.settings_file = __dirname + '/data_settings.json';
        this.settings = {
            calibration: 50
        };
    }
    
    loadSettings() {
        console.log("Loading settings...");
        try {
            this.settings = fs.readFileSync(this.settings_file);
            console.log("Settings loaded");
        }
        catch(error) {
            console.log("Settings cannot be loaded, initializing with default values");
        }
    }
    
    saveSettings() {
        console.log("Saving settings...");
        fs.writeFileSync(this.settings_file, JSON.stringify(this.settings), 'utf8');
        console.log("Settings saved !");
    }
    
    get calibration() {
        return this.settings.calibration;
    }
    
    set calibration(value) {
        this.settings.calibration = value;
        this.saveSettings();
    }
}
