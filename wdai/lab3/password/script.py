import random
import string
from pyscript import document, when
import js

def get_user_preferences():
    print("Password Generator")
    print("==================")

    length = int(document.getElementById('lengthInput').value)

    use_upper = document.getElementById('upper').checked
    use_lower = document.getElementById('lower').checked
    use_digits = document.getElementById('digits').checked
    use_special = document.getElementById('special').checked

    return length, use_upper, use_lower, use_digits, use_special

def generate_password(length, use_upper, use_lower, use_digits, use_special):
    expected_strength = 0
    char_pool = ''
    
    if use_upper:
        char_pool += string.ascii_uppercase
        expected_strength += 1
    if use_lower:
        char_pool += string.ascii_lowercase
        expected_strength += 1
    if use_digits:
        char_pool += string.digits
        expected_strength += 1
    if use_special:
        char_pool += string.punctuation
        expected_strength += 1
    if length >= 12:
        expected_strength += 1

    if not char_pool:
        raise ValueError("At least one character set must be selected!")

    password = ""
    while evaluate_strength_score(password) < expected_strength:
        password = ''.join(random.choice(char_pool) for _ in range(length))
    
    return password

def evaluate_strength_score(password):
    length = len(password)
    score = 0

    if any(c.islower() for c in password): score += 1
    if any(c.isupper() for c in password): score += 1
    if any(c.isdigit() for c in password): score += 1
    if any(c in string.punctuation for c in password): score += 1

    if length >= 12:
        score += 1

    return score

@when('click', '#generate')
def generate():
    try:
        length, use_upper, use_lower, use_digits, use_special = get_user_preferences()
        password = generate_password(length, use_upper, use_lower, use_digits, use_special)
        print(f"Generated password: {password}")
        js.alert(f"Generated password: {password}")
        password_score = evaluate_strength_score(password)
        strength_levels = {
            1: "Very Weak",
            2: "Weak",
            3: "Moderate",
            4: "Strong",
            5: "Very Strong"
        }
        strength = strength_levels.get(password_score, "Unknown")
        print(f"Password Strength: {strength}")
    except ValueError as e:
        print(f"Error: {e}")
        document.getElementById('error').innerText = f"Error: {e}"

@when('click', '#upper')
@when('click', '#lower')
@when('click', '#digits')
@when('click', '#special')
@when('input', '#lengthInput')
def resetError():
    document.getElementById('error').innerText = ""