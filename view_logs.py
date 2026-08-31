#!/usr/bin/env python3
import pandas as pd
import os

if not os.path.exists('access_log.xlsx'):
    print("ERROR: No log file found!")
    print("Run the Arduino system first to generate logs.")
    exit(1)

try:
    df = pd.read_excel('access_log.xlsx')
    
    print("=" * 70)
    print("  ACCESS LOGS")
    print("=" * 70)
    
    if len(df) == 0:
        print("  No entries yet. Enter '6666#' on keypad.")
    else:
        for _, row in df.iterrows():
            print(f"  {row['Timestamp']}  ->  {row['Event']}")
    
    print("=" * 70)
    print(f"  Total: {len(df)} entries")
    print("=" * 70)
    
    # Show summary stats
    if len(df) > 0:
        granted = len(df[df['Event'] == 'Access Granted!'])
        wrong = len(df[df['Event'] == 'Wrong Password Attempt!'])
        locked = len(df[df['Event'] == 'System Locked!'])
        reset = len(df[df['Event'] == 'System Reset'])
        
        print("\n  SUMMARY:")
        print(f"    Access Granted:   {granted}")
        print(f"    Wrong Attempts:   {wrong}")
        print(f"    System Locked:    {locked}")
        print(f"    System Reset:     {reset}")
        print("=" * 70)
        
except Exception as e:
    print(f"ERROR: {e}")
    print("Try: pip install pandas openpyxl")
