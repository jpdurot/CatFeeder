import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HistoryComponent } from './history.component';
import { HistoryRoutingModule } from './history-routing.module';
import { CoreModule } from '@app/core';
import { TranslateModule } from '@ngx-translate/core';
import { IonicModule } from 'ionic-angular';


@NgModule({
  imports: [
    CoreModule,
    TranslateModule,
    CommonModule,
    IonicModule,
    HistoryRoutingModule
  ],
  declarations: [HistoryComponent],
})
export class HistoryModule { 

}
