import scheduler from './lib/node-schedule';

export class Scheduler {
    
    constructor() {
        this.jobs = [];
        this.scheduler = scheduler;
    }
    
    clearJobs() {
        while (this.jobs.length) {
            this.jobs[0].cancel(false);
            this.jobs.shift();
        }
    }
    
    addJob(jobInfos, jobMethod) {
        const rule = new scheduler.RecurrenceRule();
        //rule.dayOfWeek = new scheduler.Range(0,6);
        //rule.year = new scheduler.Range(2018,2050);
        //rule.month = new scheduler.Range(0,11);
        //rule.day = new scheduler.Range(1,31);
        rule.hour = jobInfos.hour;
        rule.minute = jobInfos.minute;
        //rule.start = new Date();

        this.jobs.push(this.scheduler.scheduleJob({tz: 'Europe/Paris', rule: rule}, jobMethod));
    }
    
    cancelNext() {
        const nextJob = this.getNextJobInstance();
        if ( nextJob ) {
            nextJob.cancelNext(true);
        }
    }
    
    getNextJobDate() {
        const nextJob = this.getNextJobInstance();
        if ( nextJob ) {
            return nextJob.nextInvocation();
        }
        return null;
    }
    
    // Private methods
    
    getNextJobInstance() {
        let nextJob, nextDate;
        
        if (this.jobs.length === 0) {
            return null;
        } else {
            for (var j = 0; j < this.jobs.length; j++) {
                const jobDate = this.jobs[j].nextInvocation();
                if (jobDate !== null && (typeof nextDate === 'undefined' || jobDate < nextDate)) {
                    nextDate = jobDate;
                    nextJob = this.jobs[j];
                }
            }
        }
        return nextJob;
    }
    
    
    
    
}