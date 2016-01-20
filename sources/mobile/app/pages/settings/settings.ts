import {Page, NavController, Modal} from 'ionic-framework/ionic';
import {DeviceSelectionPage} from '../deviceSelection/deviceSelection';
import {IBluetoothDevice} from '../../services/bluetoothService';
import * as Storage from '../../services/storageService';

@Page({
  templateUrl: 'build/pages/settings/settings.html'
})
export class SettingsPage {
    private nav: NavController;
    private storage:Storage.StorageService;
    private settings:Storage.ISettings;
    
  constructor(nav: NavController, storage: Storage.StorageService) {
    this.nav = nav;
    this.storage = storage;
    this.settings = this.storage.getSettings();
  }
  
  selectDevice(device:any)
  {
      let modal = Modal.create(DeviceSelectionPage);
      modal.onDismiss(
          data => 
          {
              if (data !== null)
              {
                  this.settings.bluetoothDeviceName = (<IBluetoothDevice>data).name;
                  this.storage.updateSettings(this.settings);
              }
          });
      this.nav.present(modal);
  }
}
