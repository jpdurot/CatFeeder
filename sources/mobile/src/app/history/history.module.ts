import { NgModule } from '@angular/core';
import { CommonModule } from '@angular/common';
import { HistoryComponent } from './history.component';
import { HistoryRoutingModule } from './history-routing.module';
import { CoreModule } from '@app/core';
import { TranslateModule } from '@ngx-translate/core';
import { IonicModule } from 'ionic-angular';
import { SharedModule } from '@app/shared/shared.module';


@NgModule({
  imports: [
    CoreModule,
    TranslateModule,
    CommonModule,
    IonicModule,
    HistoryRoutingModule,
    SharedModule
  ],
  declarations: [HistoryComponent],
})
export class HistoryModule { 

}
