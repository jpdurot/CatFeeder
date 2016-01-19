import {Alert, Page, ViewController, NavController} from 'ionic-framework/ionic';

import {BluetoothService, IBluetoothDevice} from '../../services/bluetoothService';

@Page({
  templateUrl: 'build/pages/deviceSelection/deviceSelection.html'
})

export class DeviceSelectionPage {
    private viewController: ViewController;
    private nav:NavController;
    private isLoading:boolean;
    private bluetoothService:BluetoothService;
    devices:any;
  
  
  constructor(viewController: ViewController) {
    this.viewController = viewController;
    this.bluetoothService = new BluetoothService();
    this.isLoading = false;
    this.devices = [];
    this.getBluetoothDevices();
  }
  
  getBluetoothDevices()
  {
      this.devices = [];
      this.isLoading = true;
      /*this.devices= [
          {name: "dev1"},
          {name: "dev2"}
      ];*/
      this.bluetoothService.getDeviceList().then(
          (list) => this.devices = list,
          (error) => 
          {
            let alert = Alert.create({
                title: 'Bluetooth',
                subTitle: error,
                buttons: ['Dismiss']
            });
            this.nav.present(alert);
        });
  }
  
  onDevicesListReceived(data:any)
  {
      this.devices = data;
  }
  
  onDevicesListError(error:any)
  {
      console.error(error);
  }
  
  cancel()
  {
      this.selectDevice(null);
  }
  
  selectDevice(device:any)
  {
      this.viewController.dismiss(device);  
  }
}
