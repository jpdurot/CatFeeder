import {Page, NavController, Alert} from 'ionic-framework/ionic';

import {SettingsPage} from '../settings/settings';
import * as Pipes from '../../utils/pipes';

import * as Bluetooth from '../../services/bluetoothService';
import * as Storage from '../../services/storageService';
import * as Messaging from '../../services/messagingService';
import * as Debug from '../../services/debugService';

@Page({
  templateUrl: 'build/pages/home/home.html',
  pipes: [Pipes.BluetoothStatusToString]
})
export class HomePage {
    private nav: NavController;
    private bluetoothService: Bluetooth.BluetoothService;
    private storage: Storage.StorageService;
    private messaging: Messaging.MessagingService;
    private debug: Debug.DebugService;
    
    private settings:Storage.ISettings;
    private bluetoothInfo: Bluetooth.IBluetoothStatus;
    debugMessages:Array<Debug.IDebugMessage>;
    
  constructor(  nav: NavController, 
                bluetooth: Bluetooth.BluetoothService, 
                storage: Storage.StorageService, 
                messaging: Messaging.MessagingService,
                debug: Debug.DebugService) {
    this.nav = nav;
    this.bluetoothService = bluetooth;
    this.storage = storage;
    this.messaging = messaging;
    this.debug = debug;
    
    this.debugMessages = this.debug.Messages;
    this.debug.logDebug("Debug message");
    this.debug.logInfo("Info message");
    this.debug.logError("Error message");
    
    this.bluetoothInfo = this.bluetoothService.getStatus();
    this.messaging.messageReceived.on(this.onMessageReceived.bind(this));
    
  }
  
  onMessageReceived(message:Messaging.IMessage)
  {
      let alert = Alert.create({
                    title: 'Message received',
                    subTitle: 'Pong',
                    buttons: ['OK']
                });
      this.nav.present(alert);
  }
  
  onPageWillEnter()
  {
      this.settings = this.storage.getSettings();
    if (this.settings.bluetoothDeviceName !== null
        && this.bluetoothInfo.deviceName !== this.settings.bluetoothDeviceName
         && this.bluetoothInfo.status != Bluetooth.BluetoothConnectionStatus.Connected)
    {
        this.bluetoothService.connect(this.settings.bluetoothDeviceName).then(
            (result) => {},
            (error)=>
            {
                this.debug.logError('Unable to connect: ' + error);
                let alert = Alert.create({
                    title: 'Bluetooth',
                    subTitle: 'Unable to connect: ' + error,
                    buttons: ['Dismiss']
                });
                this.nav.present(alert);
            });
      }
  }
  
  openSettings()
  {
      this.nav.push(SettingsPage);
  }
  
  feedCats()
  {
      this.messaging.sendFeedCatsMessage()
      .then(
          () => this.debug.logDebug("Feed cats OK"),
          (error) => this.debug.logError("Error sending Feedcat message" + error)
      );
  }
  
  ping()
  {
      this.messaging.sendPingMessage()
      .then(
          () => this.debug.logDebug("Ping OK"),
          (error) => this.debug.logError("Error sending Ping message : " + error)
      );
  }
  
  getMessageClass(msg:Debug.IDebugMessage)
  {
      switch (msg.type)
      {
          case Debug.MessageType.Debug:
            return "debug";
            break;
          case Debug.MessageType.Info:
            return "info";
            break;
          case Debug.MessageType.Error:
            return "error";
            break;
      }
  }
}
