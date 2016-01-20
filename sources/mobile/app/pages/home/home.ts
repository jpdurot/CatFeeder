import {Page, NavController, Alert} from 'ionic-framework/ionic';

import {SettingsPage} from '../settings/settings';
import * as Pipes from '../../utils/pipes';

import * as Bluetooth from '../../services/bluetoothService';

import * as Storage from '../../services/storageService';

@Page({
  templateUrl: 'build/pages/home/home.html',
  pipes: [Pipes.BluetoothStatusToString]
})
export class HomePage {
    private nav: NavController;
    private bluetoothService: Bluetooth.BluetoothService;
    private storage: Storage.StorageService;
    private settings:Storage.ISettings;
    private bluetoothInfo: Bluetooth.IBluetoothStatus;
    
  constructor(nav: NavController) {
    this.nav = nav;
    this.bluetoothService = new Bluetooth.BluetoothService();
    this.storage = new Storage.StorageService();
    
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
}
