import {bind, Injectable} from 'angular2/core';


export interface IBluetoothDevice
{
    
}

export enum BluetoothConnectionStatus {NotConnected = 1, Connecting = 2, Connected = 3};

export interface IBluetoothStatus
{
    status:BluetoothConnectionStatus;
    deviceName:string;
}


@Injectable()
export class BluetoothService
{
    private static isInitialized:boolean;
    private static connectPromise:Promise<boolean>;
    private static bluetoothStatus:IBluetoothStatus;
    constructor()
    {
        if (!BluetoothService.isInitialized)
        {
            BluetoothService.bluetoothStatus = 
            {
                status: BluetoothConnectionStatus.NotConnected,
                deviceName:'Test'
            }
        }
    }
    
    getStatus():IBluetoothStatus
    {
        return BluetoothService.bluetoothStatus;
    }
    
    connect(deviceName:string):Promise<boolean>
    {
        if (BluetoothService.connectPromise != null && BluetoothService.bluetoothStatus.deviceName == deviceName)
        {
            return BluetoothService.connectPromise;
        }
        BluetoothService.bluetoothStatus.deviceName = deviceName;
        BluetoothService.bluetoothStatus.status = BluetoothConnectionStatus.Connecting;
        BluetoothService.connectPromise = new Promise<boolean>(
            function(resolve, reject)
            {
                bluetoothSerial.connect(
                    deviceName,
                    (result) => 
                    { 
                        BluetoothService.bluetoothStatus.status = (result ? BluetoothConnectionStatus.Connected : BluetoothConnectionStatus.NotConnected);
                        resolve(result)
                    },
                    (error) => 
                    {
                        BluetoothService.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                        console.error(error);
                        reject(error);
                    }
                );
            }
        )
        return BluetoothService.connectPromise;
    }
    
}

export const todoInjectables = [
  bind(BluetoothService).toClass(BluetoothService),
  bind('BluetoothService').toAlias(BluetoothService)
];