import {Page, ViewController, NavController} from 'ionic-framework/ionic';

@Page({
  templateUrl: 'build/pages/deviceSelection/deviceSelection.html'
})

export class DeviceSelectionPage {
    private viewController: ViewController;
    private nav:NavController;
    private isLoading:boolean;
    devices:any;
  
  
  constructor(viewController: ViewController) {
  //constructor(nav: NavController) {
    this.viewController = viewController;
    //this.nav = nav;
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
      bluetoothSerial.list(this.onDevicesListReceived, this.onDevicesListError);
      //this.devices.push({name : "device1"});
      //this.devices.push({name : "device2"});
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
