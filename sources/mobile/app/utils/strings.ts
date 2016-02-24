export class Strings
{
    static toHexString(bytes:UInt8Array):string
    {
        let result="";
        bytes.forEach(element => {
            result += "-";
            result += element.toString(16);
        });
        if (result.length > 0)
        {
            result = result.substring(1);
        }
        return result;
    }
    
    /*static toHexString(bytes:Array<Number>):string
    {
        return toHexString(new UInt8Array(bytes));
    } */     
}