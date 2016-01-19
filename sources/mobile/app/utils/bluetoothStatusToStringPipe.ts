import {Pipe, PipeTransform} from 'angular2/core';

@Pipe({name: 'btStatusToString'})
export class BluetoothStatusToString implements PipeTransform {
  transform(value:number, args:string[]) : any {
      switch (value)
      {
          case 1:
          return "Not connected";
          case 2:
          return "Connecting";
          case 3:
          return "Connected"
      }
    return "????";
  }
}