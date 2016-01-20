import {Page, NavController, Alert} from 'ionic-framework/ionic';

import {SettingsPage} from '../settings/settings';
import * as Pipes from '../../utils/pipes';

import * as Bluetooth from '../../services/bluetoothService';
import * as Storage from '../../services/storageService';
import * as Messaging from '../../services/messagingService';

@Page({
  templateUrl: 'build/pages/home/home.html',
  pipes: [Pipes.BluetoothStatusToString]
})
export class HomePage {
    private nav: NavController;
    private bluetoothService: Bluetooth.BluetoothService;
    private storage: Storage.StorageService;
    private messaging: Messaging.MessagingService;
    
    private settings:Storage.ISettings;
    private bluetoothInfo: Bluetooth.IBluetoothStatus;
    
  constructor(nav: NavController, bluetooth: Bluetooth.BluetoothService, storage: Storage.StorageService, messaging: Messaging.MessagingService) {
    this.nav = nav;
    this.bluetoothService = bluetooth;
    this.storage = storage;
    this.messaging = messaging;
    
    this.bluetoothInfo = this.bluetoothService.getStatus();
    
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
          () => console.log("Feed cats OK"),
          () => console.error("Error sending Feedcat message")
      );
  }
}
