import {bind, Injectable} from 'angular2/core';

import {Array} from '../utils/array';


export interface IBluetoothDevice
{
    id:string,
    class: number,
    address:string,
    name:string
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
    private static listDevicesPromise:Promise<Array<IBluetoothDevice>>;
    private static bluetoothStatus:IBluetoothStatus;
    constructor()
    {
        if (!BluetoothService.isInitialized)
        {
            BluetoothService.bluetoothStatus = 
            {
                status: BluetoothConnectionStatus.NotConnected,
                deviceName:''
            }
        }
    }
    
    getStatus():IBluetoothStatus
    {
        return BluetoothService.bluetoothStatus;
    }
    
    getDeviceList():Promise<Array<IBluetoothDevice>>
    {
        if (BluetoothService.listDevicesPromise == null)
        {
            BluetoothService.listDevicesPromise = new Promise<Array<IBluetoothDevice>>(
            function(resolve, reject)
            {
                bluetoothSerial.list(
                    (result) => 
                    { 
                        resolve(result);
                        BluetoothService.listDevicesPromise = null;
                    },
                    (error) => 
                    {
                        console.error(error);
                        reject(error);
                        BluetoothService.listDevicesPromise = null;
                    }
                );
            });
        }
        return BluetoothService.listDevicesPromise;
        
    }
    connect(deviceName:string):Promise<boolean>
    {
        BluetoothService.bluetoothStatus.deviceName = deviceName;
        BluetoothService.bluetoothStatus.status = BluetoothConnectionStatus.Connecting;
        console.debug("Connecting to "+ deviceName + "...");
        if (BluetoothService.connectPromise != null && BluetoothService.bluetoothStatus.deviceName == deviceName)
        {
            console.debug("Already connecting");
            return BluetoothService.connectPromise;
        }
        // Get devices and find MAC address corresponding to deviceName
        BluetoothService.connectPromise = new Promise<boolean>(
            (resolve, reject) =>
            {
                this.getDeviceList().then(
                    (result:Array<IBluetoothDevice>) => 
                    {
                        let device = result.find(dev => dev.name == deviceName);
                        if (device == null)
                        {
                            console.debug("No device corresponding to name "+ deviceName);
                            BluetoothService.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                            let error = "Device not found";
                            console.error(error);
                            reject(error);
                            BluetoothService.connectPromise = null;
                        }    
                        else
                        {
                            console.debug("Device found :" + device);
                            this.connectInternal(device.address).then(
                                (result) => 
                                {
                                    BluetoothService.bluetoothStatus.status = (result ? BluetoothConnectionStatus.Connected : BluetoothConnectionStatus.NotConnected);
                                    resolve(result);
                                    BluetoothService.connectPromise = null;
                                },
                                (error) => 
                                {
                                    BluetoothService.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                                    console.error(error);
                                    reject(error);
                                    BluetoothService.connectPromise = null;
                                }
                            )
                        }
                    },
                    (error) => 
                    {
                        BluetoothService.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                        console.error(error);
                        reject(error);
                        BluetoothService.connectPromise = null;
                    }
                );
            });
            return BluetoothService.connectPromise;
    }
    
    private connectInternal(macAddress:string):Promise<boolean>
    {
        console.debug("Connecting (internal) to address " + macAddress + "...");
        BluetoothService.connectPromise = new Promise<boolean>(
            function(resolve, reject)
            {
                bluetoothSerial.connect(
                    macAddress,
                    (result) => resolve(result)
                    ,
                    (error) => reject(error)
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