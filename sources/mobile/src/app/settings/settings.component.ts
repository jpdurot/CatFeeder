import { Component, OnInit } from '@angular/core';
import { finalize } from 'rxjs/operators';

import { FormBuilder, FormGroup, Validators, ValidatorFn, AbstractControl } from "@angular/forms";
import { FeederService } from "@app/shared/feeder/feeder.service";
import { AlertController, LoadingController } from "ionic-angular";

@Component({
  selector: 'app-settings',
  templateUrl: './settings.component.html',
  styleUrls: ['./settings.component.scss']
})
export class SettingsComponent implements OnInit {

  isLoading: boolean;
  settingsForm: FormGroup;

  constructor(private feederService: FeederService, 
              private formBuilder: FormBuilder,
              private alertController: AlertController,
              private loadingController: LoadingController) {
    this.createForm();
    
   }

  ngOnInit() {
    const loading = this.loadingController.create({
        content: 'Please wait...'
    });
    loading.present();
    this.feederService.getCalibration()
      .pipe(
        finalize(() => { loading.dismiss() }))
      .subscribe((result: any) => { this.settingsForm.controls.calibration.setValue(result) },
                  (err:any) => this.showError(err));
  }

  saveSettings(): void {
    const loading = this.loadingController.create({
        content: 'Please wait...'
    });
    loading.present();
    this.feederService.setCalibration(this.settingsForm.controls.calibration.value)
      .pipe(
        finalize(() => { loading.dismiss() }))
      .subscribe((result: object) => { console.log('Save settings OK') },
                  (err:any) => this.showError(err));
  }

  private createForm(): void {
    this.settingsForm = this.formBuilder.group({
      calibration: ['', Validators.compose([Validators.required, this.integerValidator(1,255)])]
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
