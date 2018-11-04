import { NgModule, ModuleWithProviders } from '@angular/core';
import { CommonModule } from '@angular/common';
import { IonicModule } from 'ionic-angular';

import { LoaderComponent } from './loader/loader.component';
import { FeederService } from "@app/shared/feeder/feeder.service";
import { DatePipeProxy } from '@app/shared/date/DatePipeProxy';

@NgModule({
  imports: [
    IonicModule,
    CommonModule
  ],
  declarations: [
    LoaderComponent,
    DatePipeProxy
  ],
  exports: [
    LoaderComponent,
    DatePipeProxy
  ],
  providers: [
    FeederService
  ]
})
export class SharedModule {
 }
