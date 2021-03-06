//-----------------------------------------------------------------------------------------------------
const binary = require('node-pre-gyp');
const path = require('path');
const binding_path = binary.find(path.resolve(path.join(__dirname,'./package.json')));

const modEventLogger = require(binding_path);
//-----------------------------------------------------------------------------------------------------


const getEnumMember = (severity) => {
    switch (severity) {
        case "error":
            return 0;
        case "warn":
            return 1;
        case "info":
            return 2;
        default:
            return 0;
    }
};


class EventLog {

    constructor(source) {
        this.source = source;
        this._logger = modEventLogger(source);
    }

    log(message, severity = 'info', code = 1000) {
        return new Promise( (resolve, reject) => {
            this._logger.log(getEnumMember(severity), message, code).then( (v) => {
                resolve(v);
            }).catch( (e) => { reject(e); });
        })
    }

}

module.exports = {
    EventLog
};