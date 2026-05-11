# Simple markdown fixer for PRODUCTION_REPORT.md

with open('PRODUCTION_REPORT.md', 'r') as f:
    lines = f.readlines()

fixed_lines = []
i = 0
while i < len(lines):
    line = lines[i]
    
    # Add blank line before headings (except first)
    if i > 0 and line.startswith('#') and fixed_lines[-1].strip():
        if not fixed_lines[-1].startswith('#') and fixed_lines[-1].strip():
            fixed_lines.append('\n')
    
    # Fix table separators - convert to minimal format
    if line.strip().startswith('|') and '-' in line and i > 0:
        if lines[i-1].strip().startswith('|'):
            # Count columns from previous line
            col_count = lines[i-1].count('|') - 1
            if col_count > 0:
                sep = '| ' + ' | '.join(['---'] * col_count) + ' |'
                fixed_lines.append(sep + '\n')
                i += 1
                continue
    
    fixed_lines.append(line)
    i += 1

# Write back
with open('PRODUCTION_REPORT.md', 'w') as f:
    f.writelines(fixed_lines)

print("✅ Basic markdown formatting fixed!")
