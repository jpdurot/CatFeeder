import {Page, NavController, Alert} from 'ionic-framework/ionic';

import {SettingsPage} from '../settings/settings';
import {BluetoothStatusToString} from '../../utils/bluetoothStatusToStringPipe';

import {BluetoothService, IBluetoothStatus, IBluetoothDevice} from '../../services/bluetoothService';

@Page({
  templateUrl: 'build/pages/home/home.html',
  pipes: [BluetoothStatusToString]
})
export class HomePage {
    private nav: NavController;
    private bluetoothService: BluetoothService;
    
    private bluetoothInfo: IBluetoothStatus;
    
  constructor(nav: NavController) {
    this.nav = nav;
    this.bluetoothService = new BluetoothService();
    this.bluetoothInfo = this.bluetoothService.getStatus();
    this.bluetoothService.connect("CatFeeder").then(
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
  
  openSettings()
  {
      this.nav.push(SettingsPage);
  }
}
