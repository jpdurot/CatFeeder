import {Injectable} from 'angular2/core';

import {BluetoothService} from './bluetoothService';

import {SimpleEvent, ISimpleEvent} from '../utils/events';

export interface IMessage
{
    code: number;
    data: Uint8Array;
}

enum MessageStatus {StartTokenExpected = 1, EscapeTokenReceived = 2, DataExpected = 3}

@Injectable()
export class MessagingService
{
    private MessageBeginToken:number = 0x41;
    private MessageEndToken:number = 0x5a;
    private MessageEscapeToken:number = 0x40;
    
    private onMessageReceived:SimpleEvent<IMessage>;
    private messageStatus: MessageStatus;
    
    private messageData: Array<number>;

    private btService:BluetoothService;
    
    public get messageReceived(): ISimpleEvent<IMessage> { return this.onMessageReceived ;}
    
    constructor(bluetoothService:BluetoothService)
    {
        this.onMessageReceived = new SimpleEvent<IMessage>();
        this.btService = bluetoothService;
        this.btService.dataReceived.on(this.dataReceivedHandler.bind(this));
        this.messageData = new Array<number>();
        this.resetMessage();
    }
    
    sendPingMessage():Promise<void>
    {
        return new Promise<void>(
            (resolve, reject) =>
            {
                this.btService.sendRaw(new Uint8Array(this.buildMessage([0x47])))
                .then(
                    () => resolve(),
                    (error) => reject(error)
                );
            }
        );
    }
    
    sendFeedCatsMessage():Promise<void>
    {
        return new Promise<void>(
            (resolve, reject) =>
            {
                this.btService.sendRaw(new Uint8Array(this.buildMessage([0x46])))
                .then(
                    () => resolve(),
                    (error) => reject(error)
                );
            }
        );
    }
    
    private buildMessage(payload:Array<number>): Array<number>
    {
        // TODO : escape tokens
        let message: number[] = new Array<number>();
        message.push(this.MessageBeginToken);
        payload.forEach((value) => message.push(value));
        message.push(this.MessageEndToken);
        return message;
    }
    
    private resetMessage()
    {
        this.messageStatus = MessageStatus.StartTokenExpected;
    }
    
    
    
    private dataReceivedHandler(data:Uint8Array)
    {
        data.forEach(
            (value, index, array) =>
            {
                if (this.messageStatus == MessageStatus.EscapeTokenReceived)
                {
                        this.messageData.push(value);
                        this.messageStatus = MessageStatus.DataExpected;
                }
                else if (value == this.MessageBeginToken)
                {
                    this.messageData = new Array<number>();
                    this.messageStatus = MessageStatus.DataExpected;
                }
                else if (value == this.MessageEndToken)
                {
                    if (this.messageStatus == MessageStatus.DataExpected)
                    {
                        if (this.messageData.length >0)
                        {
                            // full message received
                            let message:IMessage = 
                            {
                                code: this.messageData[0],
                                data: this.messageData.length==1 ? new Uint8Array(0) : new Uint8Array(this.messageData.slice(1,this.messageData.length))
                            };
                            this.onMessageReceived.trigger(message);
                            this.resetMessage();
                        }
                        
                    }
                }
                else if (value == this.MessageEscapeToken)
                {
                    if (this.messageStatus == MessageStatus.DataExpected)
                    {
                        this.messageStatus = MessageStatus.EscapeTokenReceived;
                    }
                }
                else if (this.messageStatus == MessageStatus.DataExpected)
                {
                    this.messageData.push(value);
                }
            }
        )
    }
    
    
}