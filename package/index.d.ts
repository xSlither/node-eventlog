export type Severity = "info" | "warn" | "error";

export declare class EventLog {
    public readonly source: string;

    constructor(source: string);
    log(message: string, severity?: Severity, code?: number): Promise<boolean>;
}