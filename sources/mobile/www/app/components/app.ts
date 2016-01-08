import {Component, View}               from 'angular2/core'

import {RouteConfig, RouteDefinition, RouterOutlet, ROUTER_DIRECTIVES}     from 'angular2/router'


import {Home} from './home/home'



@Component(
    {
        selector: 'app',
        template: '<h1>{{title}}</h1><router-outlet></router-outlet>',
        directives: ROUTER_DIRECTIVES
    }
)

@RouteConfig([
   { path: '/', redirectTo: ['Home'] },
   { path: '/home', as: 'Home', component: Home }
])

export class App 
{
    title: string;
    
    constructor()
    {
        this.title = "My first Angular 2 App";
    }
}


