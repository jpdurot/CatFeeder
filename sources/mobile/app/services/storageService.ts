export interface ISettings
{
    bluetoothDeviceName:string;
}

export class StorageService
{
    private SettingsKey:string = "globalSettings";
    constructor()
    {}
    
    getSettings():ISettings
    {
        let value = window.localStorage.getItem(this.SettingsKey);
        if (value === null)
        {
            return <ISettings> { bluetoothDeviceName: null };
        }
        return value && JSON.parse(value);
    }
    
    updateSettings(value:ISettings)
    {
        window.localStorage.setItem(this.SettingsKey,JSON.stringify(value));
    }
    
    /*getValue<T>(key:string):T
    {
        let value = window.localStorage.getItem(key);
        return value && JSON.parse(value);
    }
    
    setValue<T>(key:string, value: T)
    {
        window.localStorage.setItem(key,JSON.stringify(value));
    }
    
    removeValue(key: string)
    {
        window.localStorage.removeItem(key);
    }*/
    
}