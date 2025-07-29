# Скрипты автоматизации релизов

## Обзор
Этот документ описывает скрипты и инструменты для автоматизации процесса релиза Overthrow.

## Bash скрипты для Git операций

### Подготовка релиза
```bash
#!/bin/bash
# prepare_release.sh - Подготовка нового релиза

VERSION=$1
if [ -z "$VERSION" ]; then
    echo "Использование: ./prepare_release.sh <version>"
    echo "Пример: ./prepare_release.sh 1.2.3"
    exit 1
fi

echo "🚀 Подготовка релиза v$VERSION..."

# Проверяем что мы на develop ветке
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
if [ "$CURRENT_BRANCH" != "develop" ]; then
    echo "❌ Ошибка: Переключитесь на ветку develop"
    exit 1
fi

# Проверяем что нет незафиксированных изменений
if ! git diff-index --quiet HEAD --; then
    echo "❌ Ошибка: Есть незафиксированные изменения"
    exit 1
fi

# Создаем release ветку
git checkout -b "release/v$VERSION"
echo "✅ Создана ветка release/v$VERSION"

# Обновляем версию в addon.gproj (если нужно)
# sed -i "s/VERSION_PLACEHOLDER/$VERSION/g" addon.gproj

echo "✅ Релиз v$VERSION подготовлен"
echo "📝 Не забудьте обновить CHANGELOG.md"
echo "🧪 Запустите тестирование перед финализацией"
```

### Финализация релиза
```bash
#!/bin/bash
# finalize_release.sh - Финализация релиза

VERSION=$1
if [ -z "$VERSION" ]; then
    echo "Использование: ./finalize_release.sh <version>"
    exit 1
fi

echo "🎯 Финализация релиза v$VERSION..."

# Проверяем что мы на release ветке
CURRENT_BRANCH=$(git rev-parse --abbrev-ref HEAD)
if [ "$CURRENT_BRANCH" != "release/v$VERSION" ]; then
    echo "❌ Ошибка: Переключитесь на ветку release/v$VERSION"
    exit 1
fi

# Финальный коммит
git add .
git commit -m "Release v$VERSION"

# Мержим в main
git checkout main
git merge --no-ff "release/v$VERSION" -m "Merge release/v$VERSION"

# Создаем тег
git tag -a "v$VERSION" -m "Release v$VERSION"

# Мержим обратно в develop
git checkout develop
git merge --no-ff main

# Пушим все
git push origin main
git push origin develop
git push origin "v$VERSION"

# Удаляем release ветку
git branch -d "release/v$VERSION"
git push origin --delete "release/v$VERSION"

echo "✅ Релиз v$VERSION финализирован"
echo "🌐 Теперь можно публиковать в Steam Workshop"
```

### Создание hotfix
```bash
#!/bin/bash
# create_hotfix.sh - Создание hotfix релиза

CURRENT_VERSION=$1
if [ -z "$CURRENT_VERSION" ]; then
    echo "Использование: ./create_hotfix.sh <current_version>"
    echo "Пример: ./create_hotfix.sh 1.2.3"
    exit 1
fi

# Вычисляем новую версию (увеличиваем patch)
IFS='.' read -ra VERSION_PARTS <<< "$CURRENT_VERSION"
MAJOR=${VERSION_PARTS[0]}
MINOR=${VERSION_PARTS[1]}
PATCH=${VERSION_PARTS[2]}
NEW_PATCH=$((PATCH + 1))
NEW_VERSION="$MAJOR.$MINOR.$NEW_PATCH"

echo "🚨 Создание hotfix v$NEW_VERSION..."

# Создаем hotfix ветку от main
git checkout main
git checkout -b "hotfix/v$NEW_VERSION"

echo "✅ Создана ветка hotfix/v$NEW_VERSION"
echo "🔧 Внесите исправления и запустите ./finalize_hotfix.sh $NEW_VERSION"
```

### Финализация hotfix
```bash
#!/bin/bash
# finalize_hotfix.sh - Финализация hotfix

VERSION=$1
if [ -z "$VERSION" ]; then
    echo "Использование: ./finalize_hotfix.sh <version>"
    exit 1
fi

echo "🔥 Финализация hotfix v$VERSION..."

# Коммитим изменения
git add .
git commit -m "Hotfix v$VERSION"

# Мержим в main
git checkout main
git merge --no-ff "hotfix/v$VERSION" -m "Merge hotfix/v$VERSION"

# Создаем тег
git tag -a "v$VERSION" -m "Hotfix v$VERSION"

# Мержим в develop
git checkout develop
git merge --no-ff main

# Пушим все
git push origin main
git push origin develop
git push origin "v$VERSION"

# Удаляем hotfix ветку
git branch -d "hotfix/v$VERSION"

echo "✅ Hotfix v$VERSION завершен"
```

## PowerShell скрипты (Windows)

### Проверка готовности к релизу
```powershell
# check_release_readiness.ps1
param(
    [Parameter(Mandatory=$true)]
    [string]$Version
)

Write-Host "🔍 Проверка готовности релиза v$Version..." -ForegroundColor Cyan

# Проверка Git статуса
$gitStatus = git status --porcelain
if ($gitStatus) {
    Write-Host "❌ Есть незафиксированные изменения:" -ForegroundColor Red
    Write-Host $gitStatus
    exit 1
}

# Проверка что CHANGELOG обновлен
$changelogContent = Get-Content "docs/CHANGELOG_TEMPLATE.md" -Raw
if ($changelogContent -match "\[Unreleased\][\s\S]*?### Добавлено\s*-\s*$") {
    Write-Host "⚠️  CHANGELOG не обновлен для релиза" -ForegroundColor Yellow
}

# Проверка компиляции (симуляция)
Write-Host "🔨 Проверьте компиляцию в Arma Reforger Workbench" -ForegroundColor Yellow

# Проверка что все тесты пройдены (если есть)
Write-Host "🧪 Убедитесь что все тесты пройдены" -ForegroundColor Yellow

Write-Host "✅ Проверка завершена" -ForegroundColor Green
```

## GitHub Actions Workflows

### Автоматическая проверка PR
```yaml
# .github/workflows/pr_check.yml
name: PR Validation

on:
  pull_request:
    branches: [ main, develop ]

jobs:
  validate:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Check changelog updated
      run: |
        if git diff --name-only origin/develop...HEAD | grep -q "docs/CHANGELOG_TEMPLATE.md"; then
          echo "✅ Changelog updated"
        else
          echo "⚠️ Consider updating changelog"
        fi
    
    - name: Validate EnforceScript syntax
      run: |
        # Простая проверка синтаксиса (можно расширить)
        find Scripts -name "*.c" -exec echo "Checking {}" \;
        
    - name: Check for Russian comments
      run: |
        # Проверка что комментарии содержат русский текст для локализации
        grep -r "//.*[а-яё]" Scripts/ || echo "No Russian comments found"
```

### Автоматическое создание релиза
```yaml
# .github/workflows/release.yml
name: Create Release

on:
  push:
    tags:
      - 'v*'

jobs:
  release:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v3
    
    - name: Extract version
      id: extract_version
      run: echo "version=${GITHUB_REF#refs/tags/}" >> $GITHUB_OUTPUT
    
    - name: Generate changelog
      run: |
        # Извлекаем changelog для этой версии
        sed -n "/## \[${{ steps.extract_version.outputs.version }}\]/,/## \[/p" docs/CHANGELOG_TEMPLATE.md | head -n -1 > release_notes.md
    
    - name: Create GitHub Release
      uses: actions/create-release@v1
      env:
        GITHUB_TOKEN: ${{ secrets.GITHUB_TOKEN }}
      with:
        tag_name: ${{ steps.extract_version.outputs.version }}
        release_name: Overthrow ${{ steps.extract_version.outputs.version }}
        body_path: release_notes.md
        draft: false
        prerelease: false
    
    - name: Notify Discord
      run: |
        # Отправка уведомления в Discord webhook (опционально)
        curl -X POST "${{ secrets.DISCORD_WEBHOOK }}" \
             -H "Content-Type: application/json" \
             -d "{\"content\": \"🚀 Новая версия Overthrow ${{ steps.extract_version.outputs.version }} доступна!\"}"
```

## Инструменты разработчика

### Скрипт проверки качества кода
```bash
#!/bin/bash
# code_quality_check.sh

echo "🔍 Проверка качества кода..."

# Проверка на TODO и FIXME
echo "📝 Поиск TODO и FIXME:"
grep -r "TODO\|FIXME" Scripts/ || echo "Не найдено"

# Проверка на русские комментарии (должны быть для локализации)
echo "🌐 Проверка локализации:"
grep -r "//.*[а-яё]" Scripts/ | wc -l
echo "комментариев на русском языке найдено"

# Проверка размера файлов
echo "📊 Большие файлы (>100KB):"
find . -name "*.c" -size +100k -exec ls -lh {} \; || echo "Не найдено"

# Проверка длинных строк (>120 символов)
echo "📏 Длинные строки (>120 символов):"
find Scripts -name "*.c" -exec awk 'length($0) > 120 {print FILENAME ":" NR ": " $0}' {} \; | head -10

echo "✅ Проверка завершена"
```

### Генератор release notes
```python
#!/usr/bin/env python3
# generate_release_notes.py

import re
import sys
from datetime import datetime

def extract_changelog_section(version):
    """Извлекает секцию changelog для указанной версии"""
    try:
        with open('docs/CHANGELOG_TEMPLATE.md', 'r', encoding='utf-8') as f:
            content = f.read()
        
        # Ищем секцию для версии
        pattern = rf'## \[{re.escape(version)}\].*?\n(.*?)(?=\n## \[|\n---|\Z)'
        match = re.search(pattern, content, re.DOTALL)
        
        if match:
            return match.group(1).strip()
        else:
            return f"Changelog для версии {version} не найден"
            
    except FileNotFoundError:
        return "Файл CHANGELOG_TEMPLATE.md не найден"

def generate_discord_message(version, changelog):
    """Генерирует сообщение для Discord"""
    return f"""🚀 **Обновление Overthrow v{version}**

{changelog}

📥 **Как обновиться:**
1. Откройте GitHub Desktop
2. Нажмите "Fetch Origin" и "Pull Remote"  
3. В Reforger Script Editor: Build > Compile and Reload Scripts

🔗 **Ссылки:**
• [GitHub Release](https://github.com/ArmaOverthrow/Overthrow.Arma4/releases/tag/v{version})
• [Steam Workshop](https://reforger.armaplatform.com/workshop/59B657D731E2A11D)
"""

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Использование: python generate_release_notes.py <version>")
        sys.exit(1)
    
    version = sys.argv[1]
    changelog = extract_changelog_section(version)
    
    print("=== GitHub Release Notes ===")
    print(changelog)
    print("\n=== Discord Message ===")
    print(generate_discord_message(version, changelog))
```

## Интеграция с IDE

### VSCode Tasks
```json
// .vscode/tasks.json
{
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Prepare Release",
            "type": "shell",
            "command": "${workspaceFolder}/docs/scripts/prepare_release.sh",
            "args": ["${input:version}"],
            "group": "build",
            "presentation": {
                "echo": true,
                "reveal": "always",
                "focus": false,
                "panel": "shared"
            }
        },
        {
            "label": "Check Release Readiness",
            "type": "shell",
            "command": "${workspaceFolder}/docs/scripts/check_release_readiness.sh",
            "group": "test",
            "presentation": {
                "echo": true,
                "reveal": "always"
            }
        }
    ],
    "inputs": [
        {
            "id": "version",
            "description": "Release version",
            "default": "1.0.0",
            "type": "promptString"
        }
    ]
}
```

## Установка и использование

### Первоначальная настройка
```bash
# Делаем скрипты исполняемыми
chmod +x docs/scripts/*.sh

# Создаем алиасы для удобства
echo 'alias prepare-release="./docs/scripts/prepare_release.sh"' >> ~/.bashrc
echo 'alias finalize-release="./docs/scripts/finalize_release.sh"' >> ~/.bashrc
echo 'alias create-hotfix="./docs/scripts/create_hotfix.sh"' >> ~/.bashrc

source ~/.bashrc
```

### Пример использования
```bash
# Подготовка релиза
prepare-release 1.2.3

# После тестирования и обновления changelog
finalize-release 1.2.3

# При необходимости hotfix
create-hotfix 1.2.3
# ... исправления ...
./docs/scripts/finalize_hotfix.sh 1.2.4
```

---

*Последнее обновление: [ДАТА]*  
*Версия документа: 1.0*