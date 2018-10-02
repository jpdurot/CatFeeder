import { Component, OnInit, OnDestroy } from '@angular/core';
import { finalize } from 'rxjs/operators';

import { QuoteService } from './quote.service';
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

  private status:Array<string> = [ "En attente", "", "", "En cours"];



  constructor(private feederService: FeederService, 
              private formBuilder: FormBuilder,
              private alertController: AlertController) {
    this.createForm();
    console.dir(this.alertController);
   }

  ngOnInit() {
    this.getInfos();
    this.getInfosIntervalHandle = setInterval(() => this.getInfos(), 1000);
  }

  ngOnDestroy(): void {
    if (this.getInfosIntervalHandle) {
      clearInterval(this.getInfosIntervalHandle);
    }
  }


  getInfos() {
      this.feederService.getInfos()
      .pipe(
        finalize(() => { this.isLoading = false; }))
      .subscribe((result: any) => { this.infos = result.infos; this.lastFeed = { date: result.lastFeed.date, quantity: result.lastFeed.quantity / 10}; this.currentStatus = this.status[this.infos.command] },
                  (err: any) => this.showError(err));
  }

  feed(): void {
    this.isLoading = true;
    this.feederService.feed(this.feedForm.controls.requestedFood.value)
      .pipe(
        finalize(() => { this.isLoading = false; }))
      .subscribe((result: object) => { console.log('Feed OK') },
                  (err) => this.showError(err));
  }

  private createForm(): void {
    this.feedForm = this.formBuilder.group({
      requestedFood: ['', Validators.compose([Validators.required, this.integerValidator(1,40)])]
    });
  }

  private showError(errorObject: object) {
    const alert = this.alertController.create({
      title: 'Error',
      subTitle: JSON.stringify(errorObject),
      buttons: ['OK']
    });
    alert.present();
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
