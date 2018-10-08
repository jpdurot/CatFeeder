require = require('@std/esm')(module, { cjs: true, esm: 'js' });

var Scheduler = require('./scheduler').Scheduler;

var scheduler = new Scheduler();
scheduler.addJob({hour: 22, minute: 49}, () => { console.log("Job 1 has ran")});
scheduler.addJob({hour: 0, minute: 49}, () => { console.log("Job 2 has ran")});
scheduler.addJob({hour: 22, minute: 32}, () => { console.log("Job 3 has ran")});

console.log("Next job date = " + scheduler.getNextJobDate());
scheduler.cancelNext();
console.log("Next job date = " + scheduler.getNextJobDate());
scheduler.cancelNext();
console.log("Next job date = " + scheduler.getNextJobDate());
scheduler.cancelNext();
console.log("Next job date = " + scheduler.getNextJobDate());

scheduler.clearJobs();
console.log("Next job date = " + scheduler.getNextJobDate());

scheduler.addJob({hour: 22, minute: 49}, () => { console.log("Job 1 has ran")});
scheduler.addJob({hour: 0, minute: 49}, () => { console.log("Job 2 has ran")});
scheduler.addJob({hour: 22, minute: 32}, () => { console.log("Job 3 has ran")});

console.log("Next job date = " + scheduler.getNextJobDate());
scheduler.cancelNext();
console.log("Next job date = " + scheduler.getNextJobDate());
scheduler.cancelNext();
console.log("Next job date = " + scheduler.getNextJobDate());
scheduler.cancelNext();
console.log("Next job date = " + scheduler.getNextJobDate());

/*var scheduler = require('node-schedule');

const rule = new scheduler.RecurrenceRule();
rule.hour = 10;
rule.minute = 20;
rule.tz = 'Europe/Paris';
const myJob = scheduler.scheduleJob({tz: 'Europe/Paris', rule: rule }, () => console.log("Job has run"));
console.log("Next invocation = " + myJob.nextInvocation());
myJob.cancelNext(true);
console.log("Next invocation = " + myJob.nextInvocation());
myJob.cancelNext(true);
console.log("Next invocation = " + myJob.nextInvocation());*/


