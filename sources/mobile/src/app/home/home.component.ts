import { Component, OnInit, OnDestroy } from '@angular/core';
import { finalize } from 'rxjs/operators';

import { FormBuilder, FormGroup, Validators, ValidatorFn, AbstractControl } from "@angular/forms";
import { FeederService } from "@app/shared/feeder/feeder.service";
import { AlertController } from "ionic-angular";

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.scss']
})
export class HomeComponent implements OnInit, OnDestroy {
    


  quote: string;
  isLoading: boolean;
  feedForm: FormGroup;
  infos: any;
  lastFeed: any;
  getInfosIntervalHandle: any;
  currentStatus: string;

  private isAlertPresent:boolean;

  private status:Array<string> = [ "En attente", "", "", "En cours"];



  constructor(private feederService: FeederService, 
              private formBuilder: FormBuilder,
              private alertController: AlertController) {
    this.createForm();
    console.dir(this.alertController);
   }

  ngOnInit() {
    this.getInfos();
    this.startPollingInfos();
  }

  ngOnDestroy(): void {
    this.stopPollingInfos();
  }

  startPollingInfos() {
    this.stopPollingInfos
    this.getInfosIntervalHandle = setInterval(() => this.getInfos(), 1000);
  }

  stopPollingInfos() {
    if (this.getInfosIntervalHandle) {
      clearInterval(this.getInfosIntervalHandle);
    }
  }


  getInfos() {
      this.feederService.getInfos()
      .pipe(
        finalize(() => { this.isLoading = false; }))
      .subscribe((result: any) => { this.infos = result.infos; this.lastFeed = { date: result.lastFeed.date, quantity: result.lastFeed.quantity / 10}; this.currentStatus = this.status[this.infos.command] },
                  (err: any) => this.handleHttpError(err));
  }

  feed(): void {
    this.isLoading = true;
    this.feederService.feed(this.feedForm.controls.requestedFood.value)
      .pipe(
        finalize(() => { this.isLoading = false; }))
      .subscribe((result: object) => { console.log('Feed OK') },
                  (err) => this.handleHttpError(err));
  }

  private createForm(): void {
    this.feedForm = this.formBuilder.group({
      requestedFood: ['', Validators.compose([Validators.required, this.integerValidator(1,40)])]
    });
  }

  private handleHttpError(error: any) {
    if (error.status === 0) {
      this.showError("Connexion impossible");
    } else {
      this.showError(JSON.stringify(error));
    }
  }

  private showError(errorMessage: string) {
    if (!this.isAlertPresent) {
      const alert = this.alertController.create({
        title: 'Error',
        subTitle: errorMessage,
        buttons: [
          {
          text: 'OK',
          handler: () => { this.isAlertPresent = false; this.startPollingInfos(); }
        }
        ]
      });
      this.stopPollingInfos();
      this.isAlertPresent = true;
      alert.present();
    }
  }

  private integerValidator(min: Number, max: Number): ValidatorFn {
  return (control: AbstractControl): {[key: string]: any} => {
    const input = control.value,
          isInvalid  = input < min || input > max;
    if(isInvalid) 
        return { 'maxValue': {max}, 'minValue': {min} }
    else 
        return null;
  };
}

}
