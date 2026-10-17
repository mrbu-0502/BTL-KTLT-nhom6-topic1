"""
Script tao danh sach testcase cho he thong quan ly thue bao di dong.
Sinh cac file testcase .txt vao folder scripts_auto_test/testcase/
"""

from pathlib import Path

BASE_DIR = Path(__file__).resolve().parent / "testcase"
BASE_DIR.mkdir(parents=True, exist_ok=True)

testcases = [
    # =========================================================================
    # SUBTYPE TESTCASES (17 test cases)
    # =========================================================================
    {
        "filename": "TC_SUB_01_view_all.txt",
        "description": "Xem toan bo danh sach loai hinh thue bao hien co trong he thong",
        "expected": [
            "DANH SACH LOAI HINH THUE BAO",
            "SUB01",
            "Tra Truoc Economy",
            "SUB10",
            "eSIM IoT"
        ],
        "inputs": [
            "3",  # Vao menu Loai hinh
            "1",  # Xem danh sach
            "0",  # Quay lai menu chinh
            "0"   # Thoat chuong trinh
        ]
    },
    {
        "filename": "TC_SUB_02_add_success.txt",
        "description": "Them moi thanh cong mot loai hinh thue bao voi ma tu dong sinh SUB11",
        "expected": [
            "Ma loai hinh (tu dong sinh): SUB11",
            "Them loai hinh thue bao thanh cong!",
            "SUB11",
            "Goi 5G Sieu Toc"
        ],
        "inputs": [
            "3",                   # Menu SubType
            "2",                   # Them moi
            "Goi 5G Sieu Toc",     # Ten loai hinh
            "Data 5G khong gioi han dung luong", # Mo ta
            "90000",               # Cuoc phi thang
            "1",                   # Xem danh sach de kiem tra
            "0",                   # Quay lai
            "0"                    # Thoat
        ]
    },
    {
        "filename": "TC_SUB_03_add_cancel.txt",
        "description": "Huy them moi loai hinh thue bao bang cach nhap 0 tai ten loai hinh",
        "expected": [
            "=== THEM MOI LOAI HINH THUE BAO ===",
            "QUAN LY LOAI HINH THUE BAO"
        ],
        "expected_not": [
            "Them loai hinh thue bao thanh cong!"
        ],
        "inputs": [
            "3",
            "2",
            "0",  # Nhap 0 de huy
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_04_add_zero_fee.txt",
        "description": "Them moi loai hinh tra truoc voi cuoc phi thang bang 0 VND",
        "expected": [
            "Them loai hinh thue bao thanh cong!",
            "SUB11",
            "Tra Truoc 0 Dong"
        ],
        "inputs": [
            "3",
            "2",
            "Tra Truoc 0 Dong",
            "Goi cuoc mac dinh khong phi duy tri",
            "0",
            "1",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_05_search_found.txt",
        "description": "Tim kiem loai hinh thue bao ton tai theo ma SUB01",
        "expected": [
            "Ket qua tim thay:",
            "SUB01",
            "Tra Truoc Economy"
        ],
        "inputs": [
            "3",
            "3",      # Tim kiem
            "SUB01",  # Ma can tim
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_06_search_not_found.txt",
        "description": "Tim kiem loai hinh thue bao khong ton tai trong he thong (SUB99)",
        "expected": [
            "[Thong bao] Khong tim thay loai hinh co ma: SUB99"
        ],
        "inputs": [
            "3",
            "3",
            "SUB99",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_07_search_invalid_prefix.txt",
        "description": "Tim kiem voi ma loai hinh khong hop le (khong bat dau bang SUB)",
        "expected": [
            "[Loi] Ma loai hinh khong hop le (phai bat dau bang 'SUB')!"
        ],
        "inputs": [
            "3",
            "3",
            "ABC01",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_08_search_cancel.txt",
        "description": "Huy thao tac tim kiem loai hinh bang cach nhap 0",
        "expected": [
            "=== TIM KIEM LOAI HINH THEO MA ===",
            "QUAN LY LOAI HINH THUE BAO"
        ],
        "inputs": [
            "3",
            "3",
            "0",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_09_update_full.txt",
        "description": "Cap nhat toan bo thong tin cua loai hinh thue bao SUB02",
        "expected": [
            "Cap nhat loai hinh thue bao thanh cong!",
            "Ket qua tim thay:",
            "SUB02",
            "Tra Truoc Tomato New",
            "5000"
        ],
        "inputs": [
            "3",
            "4",                     # Cap nhat
            "SUB02",                 # Ma can sua
            "Tra Truoc Tomato New",  # Ten moi
            "Goi Tomato phien ban nang cap", # Mo ta moi
            "5000",                  # Cuoc moi
            "3",                     # Tim kiem kiem tra lai
            "SUB02",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_10_update_keep_defaults.txt",
        "description": "Cap nhat loai hinh giu nguyen thong tin cu bang cach nhan Enter",
        "expected": [
            "Cap nhat loai hinh thue bao thanh cong!",
            "Ket qua tim thay:",
            "SUB03",
            "Tra Truoc Student"
        ],
        "inputs": [
            "3",
            "4",
            "SUB03",
            "",      # Giu nguyen ten
            "",      # Giu nguyen mo ta
            "",      # Giu nguyen cuoc
            "3",
            "SUB03",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_11_update_not_found.txt",
        "description": "Cap nhat loai hinh khong ton tai trong he thong (SUB88)",
        "expected": [
            "[Loi] Khong tim thay loai hinh co ma: SUB88"
        ],
        "inputs": [
            "3",
            "4",
            "SUB88",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_12_update_invalid_prefix.txt",
        "description": "Cap nhat loai hinh voi ma sai dinh dang prefix (VD: KH01)",
        "expected": [
            "[Loi] Ma loai hinh khong hop le (phai bat dau bang 'SUB')!"
        ],
        "inputs": [
            "3",
            "4",
            "KH01",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_13_update_cancel.txt",
        "description": "Huy thao tac cap nhat loai hinh bang cach nhap 0",
        "expected": [
            "=== CAP NHAT LOAI HINH THUE BAO ===",
            "QUAN LY LOAI HINH THUE BAO"
        ],
        "inputs": [
            "3",
            "4",
            "0",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_14_delete_cancel_no.txt",
        "description": "Huy thao tac xoa loai hinh khi duoc hoi xac nhan (chon n)",
        "expected": [
            "Da huy thao tac xoa.",
            "Ket qua tim thay:",
            "SUB04"
        ],
        "inputs": [
            "3",
            "5",      # Xoa
            "SUB04",
            "n",      # Khong xoa
            "3",      # Tim kiem lai van con
            "SUB04",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_15_delete_confirm_yes.txt",
        "description": "Xoa thanh cong loai hinh thue bao khi xac nhan y",
        "expected": [
            "Xoa loai hinh thanh cong!",
            "[Thong bao] Khong tim thay loai hinh co ma: SUB04"
        ],
        "inputs": [
            "3",
            "5",
            "SUB04",
            "y",      # Xac nhan xoa
            "3",      # Tim kiem lai thi bao khong thay
            "SUB04",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_16_delete_not_found.txt",
        "description": "Xoa loai hinh khong ton tai trong he thong (SUB77)",
        "expected": [
            "[Loi] Khong tim thay loai hinh co ma: SUB77"
        ],
        "inputs": [
            "3",
            "5",
            "SUB77",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_SUB_17_invalid_menu_choice.txt",
        "description": "Kiem tra xu ly khi nhap lua chon menu loai hinh khong hop le",
        "expected": [
            "[Loi] Vui long chi nhap so nguyen hop le!",
            "[Loi] Gia tri phai trong khoang [0, 5]! Vui long nhap lai."
        ],
        "inputs": [
            "3",
            "abc",   # Chuoi khong phai so
            "9",     # So vuot ngoai 0-5
            "0",     # Quay lai
            "0"      # Thoat
        ]
    },

    # =========================================================================
    # COMPLAINT TESTCASES (17 test cases)
    # =========================================================================
    {
        "filename": "TC_COM_01_view_all.txt",
        "description": "Xem toan bo danh sach phieu khieu nai trong he thong",
        "expected": [
            "DANH SACH PHIEU KHIEU NAI",
            "KN01",
            "HD01",
            "KN10",
            "HD10"
        ],
        "inputs": [
            "4",  # Menu Khieu nai
            "1",  # Xem danh sach
            "0",  # Quay lai menu chinh
            "0"   # Thoat
        ]
    },
    {
        "filename": "TC_COM_02_add_success.txt",
        "description": "Them moi thanh cong mot phieu khieu nai voi ma tu dong sinh KN11",
        "expected": [
            "Them phieu khieu nai thanh cong!",
            "KN11",
            "HD01",
            "17/10/2026",
            "Toc do 4G cham tai quan Cau Giay"
        ],
        "inputs": [
            "4",
            "2",                                  # Them moi
            "HD01",                               # Ma hop dong hop le
            "17/10/2026",                         # Ngay gui
            "Toc do 4G cham tai quan Cau Giay",   # Noi dung
            "TiepNhan",                           # Trang thai
            "1",                                  # Xem lai danh sach
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_03_add_cancel_at_maHD.txt",
        "description": "Huy them moi khieu nai bang cach nhap 0 tai ma hop dong",
        "expected": [
            "=== THEM MOI PHIEU KHIEU NAI ===",
            "QUAN LY PHIEU KHIEU NAI"
        ],
        "expected_not": [
            "Them phieu khieu nai thanh cong!"
        ],
        "inputs": [
            "4",
            "2",
            "0",  # Nhap 0 de huy
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_04_add_invalid_maHD_prefix_retry.txt",
        "description": "Them moi khieu nai voi ma HD sai prefix (khong bat dau bang HD) va nhap lai dung",
        "expected": [
            "[Loi] Ma hop dong phai bat dau bang 'HD'! Vui long nhap lai.",
            "Them phieu khieu nai thanh cong!"
        ],
        "inputs": [
            "4",
            "2",
            "SAI01",                              # Sai prefix
            "HD02",                               # Nhap lai dung
            "15/10/2026",
            "Loi khong dang ky duoc goi cuoc",
            "TiepNhan",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_05_add_nonexistent_maHD_retry.txt",
        "description": "Them moi khieu nai voi ma HD chua ton tai trong he thong va nhap lai dung",
        "expected": [
            "[Canh bao] Ma hop dong HD99 chua ton tai trong he thong!",
            "Them phieu khieu nai thanh cong!"
        ],
        "inputs": [
            "4",
            "2",
            "HD99",                               # Ma HD chua ton tai
            "HD03",                               # Nhap lai ma ton tai
            "16/10/2026",
            "Sim bi khoa khong ro nguyen nhan",
            "DangXuLy",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_06_add_invalid_date_retry.txt",
        "description": "Them moi khieu nai voi ngay gui sai dinh dang hoac sai gia tri va nhap lai dung",
        "expected": [
            "sai dinh dang, hay nhap lai",
            "Them phieu khieu nai thanh cong!"
        ],
        "inputs": [
            "4",
            "2",
            "HD04",
            "35/13/2026",                         # Ngay thang sai
            "20/10/2026",                         # Nhap lai ngay hop le
            "Phan anh song dien thoai chap chon",
            "TiepNhan",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_07_search_found.txt",
        "description": "Tim kiem phieu khieu nai ton tai theo ma KN01",
        "expected": [
            "Ket qua tim thay:",
            "KN01",
            "HD01",
            "Toc do 4G cham tai khu vuc Cau Giay"
        ],
        "inputs": [
            "4",
            "3",
            "KN01",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_08_search_not_found.txt",
        "description": "Tim kiem phieu khieu nai khong ton tai trong he thong (KN99)",
        "expected": [
            "[Thong bao] Khong tim thay khieu nai co ma: KN99"
        ],
        "inputs": [
            "4",
            "3",
            "KN99",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_09_search_invalid_prefix.txt",
        "description": "Tim kiem phieu khieu nai voi ma khong bat dau bang 'KN'",
        "expected": [
            "[Loi] Ma khieu nai khong hop le (phai bat dau bang 'KN')!"
        ],
        "inputs": [
            "4",
            "3",
            "ABC01",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_10_search_cancel.txt",
        "description": "Huy thao tac tim kiem khieu nai bang cach nhap 0",
        "expected": [
            "=== TIM KIEM KHIEU NAI THEO MA ===",
            "QUAN LY PHIEU KHIEU NAI"
        ],
        "inputs": [
            "4",
            "3",
            "0",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_11_update_full.txt",
        "description": "Cap nhat toan bo thong tin cua phieu khieu nai KN02",
        "expected": [
            "Cap nhat phieu khieu nai thanh cong!",
            "Ket qua tim thay:",
            "KN02",
            "HD05",
            "22/10/2026",
            "Gia han goi cuoc that bai do he thong",
            "DangXuLy"
        ],
        "inputs": [
            "4",
            "4",                                    # Cap nhat
            "KN02",                                 # Ma khieu nai
            "HD05",                                 # Ma HD moi
            "22/10/2026",                           # Ngay moi
            "Gia han goi cuoc that bai do he thong",# Noi dung moi
            "DangXuLy",                             # Trang thai moi
            "3",                                    # Tim kiem kiem tra lai
            "KN02",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_12_update_keep_defaults.txt",
        "description": "Cap nhat phieu khieu nai giu nguyen cac gia tri cu bang Enter",
        "expected": [
            "Cap nhat phieu khieu nai thanh cong!",
            "Ket qua tim thay:",
            "KN03",
            "HD03",
            "Bi tru tien tai khoan khong ro nguyen nhan"
        ],
        "inputs": [
            "4",
            "4",
            "KN03",
            "",                                    # Giu nguyen ma HD
            "20/01/2024",                          # Ngay gui (Date::input)
            "",                                    # Giu nguyen noi dung
            "",                                    # Giu nguyen trang thai
            "3",
            "KN03",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_13_update_not_found.txt",
        "description": "Cap nhat phieu khieu nai khong ton tai trong he thong (KN88)",
        "expected": [
            "[Loi] Khong tim thay khieu nai co ma: KN88"
        ],
        "inputs": [
            "4",
            "4",
            "KN88",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_14_update_invalid_prefix.txt",
        "description": "Cap nhat phieu khieu nai voi ma khong bat dau bang 'KN'",
        "expected": [
            "[Loi] Ma khieu nai khong hop le (phai bat dau bang 'KN')!"
        ],
        "inputs": [
            "4",
            "4",
            "HD01",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_15_close_confirm_no.txt",
        "description": "Huy thao tac dong phieu khieu nai khi duoc hoi xac nhan (chon n)",
        "expected": [
            "Da huy thao tac dong phieu khieu nai.",
            "Ket qua tim thay:",
            "KN04",
            "DangXuLy"
        ],
        "inputs": [
            "4",
            "5",      # Dong khieu nai
            "KN04",
            "n",      # Khong dong
            "3",      # Kiem tra lai trang thai van giu nguyen
            "KN04",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_16_close_confirm_yes.txt",
        "description": "Dong phieu khieu nai thanh cong khi xac nhan y (chuyen trang thai thanh Dong)",
        "expected": [
            "dong phieu khieu nai thanh cong!",
            "Ket qua tim thay:",
            "KN04",
            "Dong"
        ],
        "inputs": [
            "4",
            "5",
            "KN04",
            "y",      # Xac nhan dong
            "3",      # Kiem tra lai da sang trang thai Dong
            "KN04",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_17_close_not_found.txt",
        "description": "Dong phieu khieu nai khong ton tai trong he thong (KN99)",
        "expected": [
            "[Loi] Khong tim thay khieu nai co ma: KN99"
        ],
        "inputs": [
            "4",
            "5",
            "KN99",
            "0",
            "0"
        ]
    },
    {
        "filename": "TC_COM_18_invalid_menu_choice.txt",
        "description": "Kiem tra xu ly khi nhap lua chon menu khieu nai khong hop le",
        "expected": [
            "[Loi] Vui long chi nhap so nguyen hop le!",
            "[Loi] Gia tri phai trong khoang [0, 5]! Vui long nhap lai."
        ],
        "inputs": [
            "4",
            "xyz",
            "8",
            "0",
            "0"
        ]
    }
]

def main():
    count = 0
    for tc in testcases:
        content = []
        content.append("[DESCRIPTION]")
        content.append(tc["description"])
        content.append("")
        
        if "expected" in tc and tc["expected"]:
            content.append("[EXPECTED]")
            for exp in tc["expected"]:
                content.append(exp)
            content.append("")

        if "expected_not" in tc and tc["expected_not"]:
            content.append("[EXPECTED_NOT]")
            for exp_not in tc["expected_not"]:
                content.append(exp_not)
            content.append("")

        content.append("[INPUT]")
        for inp in tc["inputs"]:
            content.append(inp)

        file_path = BASE_DIR / tc["filename"]
        with open(file_path, "w", encoding="utf-8") as f:
            f.write("\n".join(content) + "\n")
        count += 1

    print(f"Da tao thanh cong {count} file testcase trong {BASE_DIR}")

if __name__ == "__main__":
    main()

