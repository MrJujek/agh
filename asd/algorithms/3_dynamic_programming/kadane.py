def kadane(nums):
    currSum = nums[0]
    maxSum = nums[0]

    for i in range(1, len(nums)):
        if currSum < 0:
            currSum = nums[i]
        else:
            currSum += nums[i]

        maxSum = max(maxSum, currSum)
    
    return maxSum

print(kadane([-2,1,-3,4,-1,2,1,-5,4]))