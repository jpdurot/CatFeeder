import { NgModule, ModuleWithProviders } from '@angular/core';
import { CommonModule } from '@angular/common';
import { IonicModule } from 'ionic-angular';

import { LoaderComponent } from './loader/loader.component';
import { FeederService } from "@app/shared/feeder/feeder.service";

@NgModule({
  imports: [
    IonicModule,
    CommonModule
  ],
  declarations: [
    LoaderComponent
  ],
  exports: [
    LoaderComponent
  ],
  providers: [
    FeederService
  ]
})
export class SharedModule {
 }
