import * as Bluetooth from './bluetoothService';

import {SimpleEvent, ISimpleEvent} from '../utils/events';

export interface IMessage
{
    code: number;
    data: Uint8Array;
}

enum MessageStatus {StartTokenExpected = 1, EscapeTokenReceived = 2, DataExpected = 3}

export class MessagingService
{
    private MessageBeginToken:number = 0x41;
    private MessageEndToken:number = 0x5a;
    private MessageEscapeToken:number = 0x40;
    
    private onMessageReceived:SimpleEvent<IMessage>;
    private messageStatus: MessageStatus;
    
    private messageData: Uint8Array;

    private btService:Bluetooth.BluetoothService;
    
    public get messageReceived(): ISimpleEvent<IMessage> { return this.onMessageReceived ;}
    
    constructor(bluetoothService:Bluetooth.BluetoothService)
    {
        this.onMessageReceived = new SimpleEvent<IMessage>();
        this.btService = bluetoothService;
        this.btService.dataReceived.on(this.dataReceivedHandler.bind(this));
        this.messageData = new Uint8Array(0);
        this.resetMessage();
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
                        this.messageData.set([value], this.messageData.length);
                        this.messageStatus = MessageStatus.DataExpected;
                }
                else if (value == this.MessageBeginToken)
                {
                    this.messageData = new Uint8Array(0);
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
                                data: this.messageData.length==1 ? new Uint8Array(0) : this.messageData.slice(1,this.messageData.length-1)
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
                    this.messageData.set([value], this.messageData.length);
                }
            }
        )
    }
    
    
}