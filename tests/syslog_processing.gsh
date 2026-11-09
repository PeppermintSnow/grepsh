# process system logs
let target_pattern: Regex = r"ERR_[0-9]+";
let mut match_count: Int = 0;

print("Starting log analysis for pattern: ", target_pattern);

for line in read_lines("system.log") {
    if line match target_pattern {
        match_count += 1;
        
        let cleaned: String = replace(line, r"\bpassword=[^\s]+\b", "password=[REDACTED]");
        
        print("[Match #", match_count, "] ", cleaned);
    } else {
        match match_level(line) {
            "CRITICAL" => {
                print("Encountered unhandled critical event. Aborting.");
                break;
            },
            _ => {
                continue;
            }
        }
    }
}

print("Scan complete. Total matches found: ", match_count);
