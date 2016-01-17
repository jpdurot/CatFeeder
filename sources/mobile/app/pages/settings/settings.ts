import {Page, NavController, Modal} from 'ionic-framework/ionic';
import {DeviceSelectionPage} from '../deviceSelection/deviceSelection';

@Page({
  templateUrl: 'build/pages/settings/settings.html'
})
export class SettingsPage {
    private nav: NavController;
    devices:any;
  constructor(nav: NavController) {
    this.nav = nav;
    this.devices = [
        {
            "name": "device1"
        },
        {
            "name": "device2"
        }
    ]
  }
  
  selectDevice(device:any)
  {
      //this.nav.push(DeviceSelectionPage);
      let modal = Modal.create(DeviceSelectionPage);
      modal.onDismiss(data => console.log(data));
      this.nav.present(modal);
  }
}
