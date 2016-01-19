import {Page, NavController, Modal} from 'ionic-framework/ionic';
import {DeviceSelectionPage} from '../deviceSelection/deviceSelection';
import {IBluetoothDevice} from '../../services/bluetoothService';
import {StorageService, ISettings} from '../../services/storageService';

@Page({
  templateUrl: 'build/pages/settings/settings.html'
})
export class SettingsPage {
    private nav: NavController;
    private storage:StorageService;
    private settings:ISettings;
    private currentBlue
    
  constructor(nav: NavController) {
    this.nav = nav;
    this.storage = new StorageService();
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
