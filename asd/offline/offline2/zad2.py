from zad2testy import runtests

def count_inversions(A):
    ans = 0

    def merge(arr, left, mid, right):
        nonlocal ans

        n1 = mid - left + 1
        n2 = right - mid

        L = [0] * n1
        R = [0] * n2

        for i in range(n1):
            L[i] = arr[left + i]
        for j in range(n2):
            R[j] = arr[mid + 1 + j]

        i = 0
        j = 0
        k = left

        while i < n1 and j < n2:
            if L[i] <= R[j]:
                arr[k] = L[i]
                i += 1
            else:
                ans += (n1 - i)
                print(ans)
                arr[k] = R[j]
                j += 1
            k += 1

        while i < n1:
            arr[k] = L[i]
            i += 1
            k += 1

        while j < n2:
            arr[k] = R[j]
            j += 1
            k += 1

    def merge_sort(arr, left, right):
        if left < right:
            mid = (left + right) // 2

            merge_sort(arr, left, mid)
            merge_sort(arr, mid + 1, right)
            merge(arr, left, mid, right)

    merge_sort(A, 0, len(A) - 1)
    return ans


# Odkomentuj by uruchomic duze testy
runtests( count_inversions, all_tests=True )

# Zakomentuj gdy uruchamiasz duze testy
# runtests( count_inversions, all_tests=False )
