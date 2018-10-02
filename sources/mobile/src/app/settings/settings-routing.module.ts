import { NgModule } from '@angular/core';
import { Routes, RouterModule } from '@angular/router';

import { Route, extract } from '@app/core';
import { SettingsComponent } from './settings.component';

const routes: Routes = [
  Route.withShell([
    { path: '', redirectTo: '/settings', pathMatch: 'full' },
    { path: 'settings', component: SettingsComponent, data: { title: extract('Settings') } }
  ])
];

@NgModule({
  imports: [RouterModule.forChild(routes)],
  exports: [RouterModule],
  providers: []
})
export class SettingsRoutingModule { }
