import {Injectable} from 'angular2/core';


export enum MessageType 
{
    Debug = 0,
    Info = 1,
    Error = 2
}

export interface IDebugMessage
{
    type: MessageType,
    message: string
}

@Injectable()
export class DebugService
{
    private messages:Array<IDebugMessage>;
    
    get Messages():Array<IDebugMessage>
    {
          return this.messages;  
    }
    
    constructor()
    {
        this.messages = new Array<IDebugMessage>();
    }
    
    logDebug(message:string)
    {
        this.log(MessageType.Debug, message);
    }
    
    logInfo(message:string)
    {
        this.log(MessageType.Info, message);
    }
    
    logError(message:string)
    {
        this.log(MessageType.Error, message);
    }
    
    private log(type:MessageType, message: string)
    {
        this.messages.unshift({type: type, message: message});
    }
}