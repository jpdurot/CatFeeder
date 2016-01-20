import {Injectable} from 'angular2/core';

import {Array} from '../utils/array';

import * as Events from '../utils/events';


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
    private isInitialized:boolean;
    private connectPromise:Promise<boolean>;
    private listDevicesPromise:Promise<Array<IBluetoothDevice>>;
    private bluetoothStatus:IBluetoothStatus;
    private onDataReceived:Events.SimpleEvent<Uint8Array>;
    
    public get dataReceived(): Events.SimpleEvent<Uint8Array> {return this.onDataReceived;}
    
    constructor()
    {
        this.bluetoothStatus = 
        {
            status: BluetoothConnectionStatus.NotConnected,
            deviceName:''
        }
        this.onDataReceived = new Events.SimpleEvent<Uint8Array>();
        bluetoothSerial.subscribeRawData(
            (data) => this.onDataReceived.trigger(new Uint8Array(data)),
            (error) => console.log("Error during data reception : " + error)
        );
    }
    
    getStatus():IBluetoothStatus
    {
        return this.bluetoothStatus;
    }
    
    sendRaw(data: Uint8Array):Promise<void>
    {
        return new Promise<void>(
            function (resolve, reject)
            {
                bluetoothSerial.sendRaw(data, resolve, reject);
            }
        );
    }
    
    getDeviceList():Promise<Array<IBluetoothDevice>>
    {
        if (this.listDevicesPromise == null)
        {
            this.listDevicesPromise = new Promise<Array<IBluetoothDevice>>(
            function(resolve, reject)
            {
                bluetoothSerial.list(
                    (result) => 
                    { 
                        resolve(result);
                        this.listDevicesPromise = null;
                    },
                    (error) => 
                    {
                        console.error(error);
                        reject(error);
                        this.listDevicesPromise = null;
                    }
                );
            });
        }
        return this.listDevicesPromise;
        
    }
    connect(deviceName:string):Promise<boolean>
    {
        this.bluetoothStatus.deviceName = deviceName;
        this.bluetoothStatus.status = BluetoothConnectionStatus.Connecting;
        console.debug("Connecting to "+ deviceName + "...");
        if (this.connectPromise != null && this.bluetoothStatus.deviceName == deviceName)
        {
            console.debug("Already connecting");
            return this.connectPromise;
        }
        // Get devices and find MAC address corresponding to deviceName
        this.connectPromise = new Promise<boolean>(
            (resolve, reject) =>
            {
                this.getDeviceList().then(
                    (result:Array<IBluetoothDevice>) => 
                    {
                        let device = result.find(dev => dev.name == deviceName);
                        if (device == null)
                        {
                            console.debug("No device corresponding to name "+ deviceName);
                            this.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                            let error = "Device not found";
                            console.error(error);
                            reject(error);
                            this.connectPromise = null;
                        }    
                        else
                        {
                            console.debug("Device found :");
                            console.dir(device);
                            this.connectInternal(device.address).then(
                                (result) => 
                                {
                                    this.bluetoothStatus.status = (result ? BluetoothConnectionStatus.Connected : BluetoothConnectionStatus.NotConnected);
                                    resolve(result);
                                    this.connectPromise = null;
                                },
                                (error) => 
                                {
                                    this.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                                    console.error(error);
                                    reject(error);
                                    this.connectPromise = null;
                                }
                            )
                        }
                    },
                    (error) => 
                    {
                        this.bluetoothStatus.status = BluetoothConnectionStatus.NotConnected;
                        console.error(error);
                        reject(error);
                        this.connectPromise = null;
                    }
                );
            });
            return this.connectPromise;
    }
    
    private connectInternal(macAddress:string):Promise<boolean>
    {
        console.debug("Connecting (internal) to address " + macAddress + "...");
        this.connectPromise = new Promise<boolean>(
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
        return this.connectPromise;
    }
}