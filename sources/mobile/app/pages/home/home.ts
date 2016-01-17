import {Page, NavController} from 'ionic-framework/ionic';

import {SettingsPage} from '../settings/settings';

@Page({
  templateUrl: 'build/pages/home/home.html'
})
export class HomePage {
    private nav: NavController;
  constructor(nav: NavController) {
    this.nav = nav;
  }
  
  openSettings()
  {
      this.nav.push(SettingsPage);
  }
}
