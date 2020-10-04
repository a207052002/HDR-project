使用 make 編譯，請務必確認環境需求：armadillo opencv3
編譯環境預設是 linux 並沒有針對 windows 請注意，如果有需求可以自行修改編譯參數好讓源碼可以在非 windows 下編過

執行檔也只有在 linux 下能運作

編譯完後會出現 hdr 的 binary 執行檔

將圖片放於特定目錄，images 資料夾內有內建一組資料
在圖片的同一個資料夾，編輯文字檔 img_list.txt 檔名必須完全相同
裡面按照
檔名1 曝光時間倒數1
檔名2 曝光時間倒數2
檔名3 曝光時間倒數3
...
排序


執行 ./hdr [圖片目錄]
ex. ./hdr ./images

結果的原始 hdr 亮度會存成 hdr_result.hdr
tone mapping 過後的結果存成 tone_mapping.jpg

參數上，lambda : 50
samples 數量 : 500 (如果要 input rows * cols 數量低於 500 的圖請務必修改源碼重新編譯)
如果想要修改相關參數也請必須修改源碼，目前使用的版本是我認為最好的 RHDR 結果，故沒有特別給予調整參數的選項