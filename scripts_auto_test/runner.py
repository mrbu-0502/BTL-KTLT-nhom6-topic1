"""
=============================================================================
AUTOMATED TEST RUNNER CHO HE THONG QUAN LY THUE BAO DI DONG (NHOM 6)
Modules test: Loai hinh thue bao (SubType) & Khieu nai (Complaint)
=============================================================================
"""

import os
import sys
import time
import shutil
import subprocess
import argparse
from datetime import datetime
from pathlib import Path

# Thiet lap mau console ANSI tren Windows
os.system("")

COLOR_RESET = "\033[0m"
COLOR_GREEN = "\033[92m"
COLOR_RED = "\033[91m"
COLOR_YELLOW = "\033[93m"
COLOR_CYAN = "\033[96m"
COLOR_BOLD = "\033[1m"


class TestCase:
    def __init__(self, file_path):
        self.file_path = Path(file_path)
        self.name = self.file_path.stem
        self.description = ""
        self.inputs = []
        self.expected = []
        self.expected_not = []
        self.parse_file()

    def parse_file(self):
        with open(self.file_path, "r", encoding="utf-8", errors="replace") as f:
            content = f.read()

        lines = content.splitlines()
        current_section = None

        has_sections = any(
            line.strip().upper() in ("[DESCRIPTION]", "[EXPECTED]", "[EXPECTED_NOT]", "[INPUT]")
            for line in lines
        )

        if not has_sections:
            # File chua raw input
            self.description = f"Test case {self.name}"
            self.inputs = lines
            return

        for line in lines:
            line_strip = line.strip()
            if line_strip.upper() == "[DESCRIPTION]":
                current_section = "DESCRIPTION"
                continue
            elif line_strip.upper() == "[EXPECTED]":
                current_section = "EXPECTED"
                continue
            elif line_strip.upper() == "[EXPECTED_NOT]":
                current_section = "EXPECTED_NOT"
                continue
            elif line_strip.upper() == "[INPUT]":
                current_section = "INPUT"
                continue

            if current_section == "DESCRIPTION":
                if not self.description:
                    self.description = line.strip()
                else:
                    self.description += " " + line.strip()
            elif current_section == "EXPECTED":
                if line_strip:
                    self.expected.append(line_strip)
            elif current_section == "EXPECTED_NOT":
                if line_strip:
                    self.expected_not.append(line_strip)
            elif current_section == "INPUT":
                self.inputs.append(line)

        if not self.description:
            self.description = self.name


class TestRunner:
    def __init__(self, root_dir=None, filter_pattern=None, timeout=10, preserve_data=True):
        if root_dir is None:
            # scripts_auto_test la thu muc hien tai chua runner.py
            self.script_dir = Path(__file__).resolve().parent
            self.project_root = self.script_dir.parent
        else:
            self.project_root = Path(root_dir).resolve()
            self.script_dir = self.project_root / "scripts_auto_test"

        self.testcase_dir = self.script_dir / "testcase"
        self.logs_dir = self.script_dir / "logs"
        self.data_dir = self.project_root / "data"
        self.backup_dir = self.script_dir / ".backup_data"
        self.exe_path = self.project_root / "app.exe"
        self.filter_pattern = filter_pattern
        self.timeout = timeout
        self.preserve_data = preserve_data

        # Tao cac thu muc neu chua co
        self.testcase_dir.mkdir(parents=True, exist_ok=True)
        self.logs_dir.mkdir(parents=True, exist_ok=True)

    def backup_original_data(self):
        """Sao luu thu muc data goc truoc khi chay test"""
        if self.backup_dir.exists():
            shutil.rmtree(self.backup_dir)
        if self.data_dir.exists():
            shutil.copytree(self.data_dir, self.backup_dir)

    def restore_data(self):
        """Khoi phuc lai du lieu tu thu muc backup de cac test khong anh huong lan nhau"""
        if self.backup_dir.exists():
            for item in self.backup_dir.iterdir():
                dest = self.data_dir / item.name
                if item.is_file():
                    shutil.copy2(item, dest)

    def clean_backup(self):
        """Xoa thu muc backup tam"""
        if self.backup_dir.exists():
            shutil.rmtree(self.backup_dir, ignore_errors=True)

    def find_testcases(self):
        """Tim tat ca file testcase .txt trong folder testcase"""
        if not self.testcase_dir.exists():
            return []
        files = sorted(self.testcase_dir.glob("*.txt"))
        if self.filter_pattern:
            files = [f for f in files if self.filter_pattern.lower() in f.stem.lower()]
        return [TestCase(f) for f in files]

    def run_single_test(self, testcase: TestCase):
        """Thuc thi mot testcase va kiem tra ket qua"""
        if self.preserve_data:
            self.restore_data()

        input_data = "\n".join(testcase.inputs) + "\n"
        start_time = time.time()
        status = "PASS"
        failure_reasons = []
        stdout = ""
        stderr = ""

        try:
            proc = subprocess.Popen(
                [str(self.exe_path)],
                cwd=str(self.project_root),
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                encoding="utf-8",
                errors="replace"
            )
            stdout, stderr = proc.communicate(input=input_data, timeout=self.timeout)
            duration = time.time() - start_time

            if proc.returncode != 0:
                status = "FAIL"
                failure_reasons.append(f"Chuong trinh thoat voi ma loi: {proc.returncode}")

            # Kiem tra cac chuoi ky vong [EXPECTED]
            for exp in testcase.expected:
                if exp not in stdout:
                    status = "FAIL"
                    failure_reasons.append(f"Thieu chuoi mong doi: '{exp}'")

            # Kiem tra cac chuoi khong duoc xuat hien [EXPECTED_NOT]
            for exp_not in testcase.expected_not:
                if exp_not in stdout:
                    status = "FAIL"
                    failure_reasons.append(f"Xuat hien chuoi khong mong muon: '{exp_not}'")

        except subprocess.TimeoutExpired:
            proc.kill()
            stdout, stderr = proc.communicate()
            duration = time.time() - start_time
            status = "TIMEOUT"
            failure_reasons.append(f"Test case bi treo qua thoi gian quy dinh ({self.timeout}s)")
        except Exception as e:
            duration = time.time() - start_time
            status = "ERROR"
            failure_reasons.append(f"Loi ngoai le he thong: {str(e)}")

        return {
            "name": testcase.name,
            "description": testcase.description,
            "status": status,
            "duration": duration,
            "reasons": failure_reasons,
            "input": input_data,
            "stdout": stdout,
            "stderr": stderr
        }

    def run_all(self):
        """Chay toan bo test suite"""
        print(f"\n{COLOR_BOLD}{COLOR_CYAN}======================================================================{COLOR_RESET}")
        print(f"{COLOR_BOLD}{COLOR_CYAN}         CHUONG TRINH CHAY TESTCASE TU DONG - BTL KTLT NHOM 6         {COLOR_RESET}")
        print(f"{COLOR_BOLD}{COLOR_CYAN}======================================================================{COLOR_RESET}")

        if not self.exe_path.exists():
            print(f"{COLOR_RED}[Loi] Khong tim thay file thuc thi: {self.exe_path}{COLOR_RESET}")
            print("Vui long build project truoc (g++ -std=c++17 -I./include src/*.cpp -o app.exe).")
            return False

        testcases = self.find_testcases()
        if not testcases:
            print(f"{COLOR_YELLOW}[Canh bao] Khong tim thay testcase nao trong {self.testcase_dir}{COLOR_RESET}")
            return False

        print(f"Thu muc test:  {self.testcase_dir}")
        print(f"File chuong trinh: {self.exe_path}")
        print(f"Tong so testcase: {len(testcases)}\n")

        self.backup_original_data()

        results = []
        passed = 0
        failed = 0
        timeouts = 0
        errors = 0

        total_start = time.time()

        for idx, tc in enumerate(testcases, 1):
            print(f"[{idx:02d}/{len(testcases):02d}] Dang chay {tc.name}...", end=" ", flush=True)
            res = self.run_single_test(tc)
            results.append(res)

            st = res["status"]
            dur = res["duration"]

            if st == "PASS":
                passed += 1
                print(f"{COLOR_GREEN}[PASS]{COLOR_RESET} ({dur:.2f}s)")
            elif st == "FAIL":
                failed += 1
                print(f"{COLOR_RED}[FAIL]{COLOR_RESET} ({dur:.2f}s)")
                for r in res["reasons"]:
                    print(f"       -> {COLOR_YELLOW}{r}{COLOR_RESET}")
            elif st == "TIMEOUT":
                timeouts += 1
                print(f"{COLOR_RED}[TIMEOUT]{COLOR_RESET} (>{self.timeout}s)")
            else:
                errors += 1
                print(f"{COLOR_RED}[ERROR]{COLOR_RESET}")

        total_duration = time.time() - total_start

        # Khoi phuc lai data goc ban dau
        self.restore_data()
        self.clean_backup()

        # In tong ket
        print(f"\n{COLOR_BOLD}----------------------------------------------------------------------{COLOR_RESET}")
        print(f"{COLOR_BOLD}KET QUA KIEM THU:{COLOR_RESET}")
        print(f"  Tong so testcase : {len(testcases)}")
        print(f"  Thanh cong (PASS): {COLOR_GREEN}{passed}{COLOR_RESET}")
        print(f"  That bai (FAIL)  : {COLOR_RED}{failed}{COLOR_RESET}")
        if timeouts > 0:
            print(f"  Qua thoi gian    : {COLOR_YELLOW}{timeouts}{COLOR_RESET}")
        if errors > 0:
            print(f"  Loi he thong     : {COLOR_RED}{errors}{COLOR_RESET}")
        print(f"  Tong thoi gian   : {total_duration:.2f}s")
        print(f"{COLOR_BOLD}----------------------------------------------------------------------{COLOR_RESET}")

        # Ghi log ra folder logs
        log_file = self.save_log(results, total_duration, passed, failed, timeouts, errors)
        print(f"\n{COLOR_CYAN}[Thong tin] Log chi tiet da duoc luu tai:{COLOR_RESET}")
        print(f"  -> {log_file}")
        print(f"  -> {self.logs_dir / 'latest.log'}\n")

        return failed == 0 and timeouts == 0 and errors == 0

    def save_log(self, results, total_duration, passed, failed, timeouts, errors):
        """Luu ket qua kiem thu chi tiet ra thu muc logs"""
        timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
        log_filename = f"test_run_{timestamp}.log"
        log_path = self.logs_dir / log_filename
        latest_path = self.logs_dir / "latest.log"

        lines = []
        lines.append("=" * 80)
        lines.append("                      BAO CAO KIEM THU TU DONG")
        lines.append(f"Thoi gian chay: {datetime.now().strftime('%d/%m/%Y %H:%M:%S')}")
        lines.append(f"Chuong trinh  : {self.exe_path}")
        lines.append("=" * 80)
        lines.append(f"Tong so testcase : {len(results)}")
        lines.append(f"Passed           : {passed}")
        lines.append(f"Failed           : {failed}")
        lines.append(f"Timeout          : {timeouts}")
        lines.append(f"Error            : {errors}")
        lines.append(f"Tong thoi gian   : {total_duration:.2f}s")
        lines.append("-" * 80)
        lines.append(f"{'STT':<4} | {'TEN TESTCASE':<30} | {'KET QUA':<8} | {'THOI GIAN':<10} | {'MO TA'}")
        lines.append("-" * 80)

        for idx, res in enumerate(results, 1):
            st = res["status"]
            dur = f"{res['duration']:.2f}s"
            name = res["name"]
            desc = res["description"]
            lines.append(f"{idx:<4} | {name:<30} | {st:<8} | {dur:<10} | {desc}")

        lines.append("=" * 80)
        lines.append("\nCHI TIET TUNG TESTCASE:\n")

        for idx, res in enumerate(results, 1):
            lines.append("=" * 80)
            lines.append(f"[{idx:02d}] TESTCASE: {res['name']}")
            lines.append(f"Mo ta    : {res['description']}")
            lines.append(f"Ket qua  : {res['status']} ({res['duration']:.2f}s)")
            if res["reasons"]:
                lines.append("Ly do that bai:")
                for r in res["reasons"]:
                    lines.append(f"  - {r}")

            lines.append("\n[DU LIEU DAU VAO (STDIN)]:")
            for in_line in res["input"].splitlines():
                lines.append(f"  > {in_line}")

            lines.append("\n[KET QUA DAU RA (STDOUT)]:")
            for out_line in res["stdout"].splitlines():
                lines.append(f"  {out_line}")

            if res["stderr"]:
                lines.append("\n[LOI CHUONG TRINH (STDERR)]:")
                for err_line in res["stderr"].splitlines():
                    lines.append(f"  {err_line}")

            lines.append("\n")

        log_content = "\n".join(lines)

        with open(log_path, "w", encoding="utf-8") as f:
            f.write(log_content)

        with open(latest_path, "w", encoding="utf-8") as f:
            f.write(log_content)

        return log_path


def main():
    parser = argparse.ArgumentParser(description="Runner kiem thu tu dong cho BTL-KTLT Nhom 6")
    parser.add_argument("--filter", "-f", help="Loc testcase theo ten (vi du: SUB, COM, TC_SUB_01)")
    parser.add_argument("--timeout", "-t", type=int, default=10, help="Timeout cho moi testcase (giay)")
    parser.add_argument("--no-preserve-data", action="store_true", help="Khong khoi phuc lai database sau moi test")
    args = parser.parse_args()

    runner = TestRunner(
        filter_pattern=args.filter,
        timeout=args.timeout,
        preserve_data=not args.no_preserve_data
    )

    success = runner.run_all()
    sys.exit(0 if success else 1)


if __name__ == "__main__":
    main()

