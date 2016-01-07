import {Component, View}               from 'angular2/core'

import {RouteConfig, RouteDefinition, RouterOutlet}     from 'angular2/router'

import {Home} from './home/home'



@Component(
    {
        selector: 'app',
        template: '<h1>{{title}}</h1><router-outlet></router-outlet>'
    }
)

@RouteConfig([
   { path: '/', redirectTo: ['/home'] },
   { path: '/home', as: 'home', component: Home }
])

export class App 
{
    title: string;
    
    constructor()
    {
        this.title = "My first Angular 2 App";
    }
}


