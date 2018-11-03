import { Component, OnInit } from '@angular/core';
import { FeederService } from "@app/shared/feeder/feeder.service";
import { AlertController, LoadingController } from 'ionic-angular';
import { finalize } from 'rxjs/operators';
import { map } from 'lodash'; 

@Component({
  selector: 'app-history',
  templateUrl: './history.component.html',
  styleUrls: ['./history.component.scss']
})
export class HistoryComponent implements OnInit {

  private feedsHistory: Object;

  constructor(private feederService: FeederService,
              private alertController: AlertController,
              private loadingController: LoadingController) { }

  ngOnInit() {
    const loading = this.loadingController.create({
      content: 'Please wait...'
    });
    loading.present();
    this.feederService.getHistory()
      .pipe(
        finalize(() => { loading.dismiss() }))
      .subscribe((result: any) => { this.feedsHistory = this.mapToFeedsHistory(result);  },
                  (err:any) => this.showError(err));
    }

    private mapToFeedsHistory(wsResult: any) {
      return map(wsResult,
          (value: any, key: any) => {   
            return { date: key, 
                    feeds: map(value, 
                              (v: any)=> { 
                                return {date: v.date, quantity: v.quantity / 10};
                              })
                    }
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

}
