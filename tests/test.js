const { EventLog } = require('node-eventlog');


const Test = async () => {
    const logger = new EventLog('MyTestApplication');
    const result = await logger.log('Test');
    console.log(result);
};

Test();