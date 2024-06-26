沒有使用互鎖的版本
在沒有使用互鎖的版本中，多個線程在更新 account_balance 時可能會互相干擾。由於 account_balance 的讀取和更新不是原子的操作，因此可能發生如下情況：

一個線程讀取了 account_balance，但在更新它之前，另一個線程也讀取了相同的值並更新了它。結果是，第一個線程的更新操作覆蓋了第二個線程的更新，導致 account_balance 的值不正確。
這種競爭條件導致的數據不一致性使得 account_balance 的最終值不正確。

使用互鎖的版本
在使用互鎖的版本中，我們使用了 pthread_mutex_lock 和 pthread_mutex_unlock 來確保每次對 account_balance 的訪問是互斥的。在任何時刻，只有一個線程可以修改 account_balance，這樣避免了競爭條件。結果，account_balance 的最終值是正確的。

結論
多線程程序中，當多個線程需要訪問和修改共享資源時，如果不採取適當的同步措施，會導致競爭條件，從而引發數據的不一致性。使用互鎖是保護共享資源的一種有效方法，能夠確保多線程環境下的數據一致性。在實際開發中，應根據需求合理使用同步機制來避免競爭條件。
