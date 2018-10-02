import { Injectable } from '@angular/core';
import { Observable, of } from 'rxjs';
import { map, catchError } from 'rxjs/operators';
import { Component } from "@angular/core";
import { HttpClient } from "@angular/common/http";

const routes = {
  calibration: '/api/calibration',
  feed: '/api/feed',
  infos: '/api/infos'
};



@Injectable()
export class FeederService {

  constructor(private httpClient: HttpClient) { }

  feed(food: number): Observable<Object> {
      const data:  object = {
          requestedFood: food
      };
    return this.httpClient
      .post(routes.feed, data);
  }

  setCalibration(calibrationValue: number): Observable<Object> {
      const data:  object = {
          value: calibrationValue
      };
    return this.httpClient
      .post(routes.calibration, data);
  }

  getCalibration(): Observable<Object> {
      
    return this.httpClient
      .get(routes.calibration)
      .pipe(
        map((body: any) => body.calibration)
      );
  }

  getInfos(): Observable<Object> {
      
    return this.httpClient
      .get(routes.infos);
      /*.pipe(
        map((body: any) => body.calibration)
      );*/
  }

  

}
