import {Page, NavController} from 'ionic-framework/ionic';

@Page({
  templateUrl: 'build/pages/settings/settings.html'
})
export class SettingsPage {
    private nav: NavController;
  constructor(nav: NavController) {
    this.nav = nav;
  }
}
