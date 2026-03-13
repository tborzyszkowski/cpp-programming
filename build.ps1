#Requires -Version 5.1
<#
.SYNOPSIS
    Skrypt do kompilacji PlantUML, budowania i uruchamiania programów C++ oraz testów.

.DESCRIPTION
    - Kompiluje wszystkie pliki .puml do PNG w odpowiednich katalogach
    - Kompiluje i uruchamia programy C++ (g++)
    - Buduje i uruchamia testy jednostkowe (CMake + Google Test)

.PARAMETER Task
    all        – wszystko (domyślnie)
    diagrams   – tylko PlantUML → PNG
    programs   – tylko programy C++
    tests      – tylko testy jednostkowe

.EXAMPLE
    .\build.ps1
    .\build.ps1 -Task diagrams
    .\build.ps1 -Task programs
    .\build.ps1 -Task tests
#>
param(
    [ValidateSet("all", "diagrams", "programs", "tests")]
    [string]$Task = "all"
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

# ─────────────────────────────────────────────
#  Kolory pomocnicze
# ─────────────────────────────────────────────
function Write-Header([string]$msg) {
    Write-Host "`n══════════════════════════════════════════════" -ForegroundColor Cyan
    Write-Host "  $msg" -ForegroundColor Cyan
    Write-Host "══════════════════════════════════════════════" -ForegroundColor Cyan
}
function Write-Ok([string]$msg)   { Write-Host "  [OK]  $msg" -ForegroundColor Green  }
function Write-Err([string]$msg)  { Write-Host "  [ERR] $msg" -ForegroundColor Red    }
function Write-Info([string]$msg) { Write-Host "  [--]  $msg" -ForegroundColor Gray   }

$RootDir = $PSScriptRoot

# ═══════════════════════════════════════════════════════════════
#  1. PLANTUM → PNG
# ═══════════════════════════════════════════════════════════════
function Invoke-Diagrams {
    Write-Header "Kompilacja PlantUML → PNG"

    # Szukaj plantuml.jar (kolejność: projekt → sąsiednie repozytoria → rozszerzenie VS Code)
    $SearchPaths = @(
        (Join-Path $RootDir "plantuml.jar"),
        (Join-Path $RootDir ".." "plantuml.jar"),
        (Join-Path $env:USERPROFILE ".vscode\extensions\jebbs.plantuml-*\plantuml.jar"),
        "C:\ProgramData\chocolatey\lib\plantuml\tools\plantuml.jar",
        "C:\tools\plantuml\plantuml.jar"
    )

    $PlantUmlJar = $null
    foreach ($pattern in $SearchPaths) {
        $found = Resolve-Path $pattern -ErrorAction SilentlyContinue | Select-Object -Last 1
        if ($found) { $PlantUmlJar = $found.Path; break }
    }

    if (-not $PlantUmlJar) {
        Write-Err "Nie znaleziono plantuml.jar. Umieść go w $RootDir lub zainstaluj rozszerzenie VS Code 'PlantUML'."
        return
    }
    Write-Info "Używam: $PlantUmlJar"

    $pumlFiles = Get-ChildItem -Path $RootDir -Filter "*.puml" -Recurse
    if ($pumlFiles.Count -eq 0) {
        Write-Info "Brak plików .puml."
        return
    }

    foreach ($puml in $pumlFiles) {
        $outDir = $puml.DirectoryName
        Write-Info "$($puml.FullName.Replace($RootDir, '.')) → $($outDir.Replace($RootDir, '.'))"
        try {
            $result = & java -jar $PlantUmlJar -tpng -o $outDir $puml.FullName 2>&1
            if ($LASTEXITCODE -ne 0) {
                Write-Err "Błąd kompilacji: $result"
            } else {
                $png = Join-Path $outDir ([System.IO.Path]::GetFileNameWithoutExtension($puml.Name) + ".png")
                Write-Ok "Wygenerowano: $($png.Replace($RootDir, '.'))"
            }
        } catch {
            Write-Err $_
        }
    }
}

# ═══════════════════════════════════════════════════════════════
#  2. PROGRAMY C++ (g++)
# ═══════════════════════════════════════════════════════════════
function Invoke-Programs {
    Write-Header "Kompilacja i uruchamianie programów C++"

    # Każdy podkatalog z main.cpp (poza 02_unit_tests)
    $programs = @(
        @{ Dir = "01_OOP\01_classes\src";      Name = "classes";       Files = "main.cpp" },
        @{ Dir = "01_OOP\03_copy\src";          Name = "copy";          Files = "main.cpp" },
        @{ Dir = "01_OOP\04_constructors\src";  Name = "constructors";  Files = "main.cpp" },
        @{ Dir = "01_OOP\05_move_semantics\src";Name = "move_semantics";Files = "main.cpp" },
        @{ Dir = "01_OOP\06_const\src";         Name = "const_demo";    Files = "main.cpp" },
        @{ Dir = "01_OOP\07_static_members\src";Name = "static_members";Files = "main.cpp" },
        @{ Dir = "01_OOP\08_friend\src";        Name = "friend_demo";   Files = "main.cpp" },
        @{ Dir = "01_OOP\09_stack_heap\src";    Name = "stack_heap";    Files = "main.cpp" }
    )

    $gpp = Get-Command "g++" -ErrorAction SilentlyContinue
    if (-not $gpp) {
        Write-Err "Nie znaleziono g++. Zainstaluj MinGW-w64 lub MSYS2."
        return
    }

    foreach ($prog in $programs) {
        $srcDir  = Join-Path $RootDir $prog.Dir
        $exeName = $prog.Name + ".exe"
        $exePath = Join-Path $srcDir $exeName

        Write-Header "Program: $($prog.Name)"

        if (-not (Test-Path $srcDir)) {
            Write-Err "Katalog nie istnieje: $srcDir"
            continue
        }

        # Kompilacja
        Write-Info "Kompilacja → $exeName"
        $srcFile = Join-Path $srcDir $prog.Files
        try {
            $out = & g++ -std=c++17 -Wall -Wextra -o $exePath $srcFile 2>&1
            if ($LASTEXITCODE -ne 0) {
                Write-Err "Błąd kompilacji:`n$out"
                continue
            }
            Write-Ok "Skompilowano: $exeName"
        } catch {
            Write-Err $_
            continue
        }

        # Uruchomienie
        Write-Info "Uruchamianie $exeName"
        Write-Host "──────────────────────────────────────────────" -ForegroundColor DarkGray
        & $exePath
        Write-Host "──────────────────────────────────────────────" -ForegroundColor DarkGray
        Write-Ok "Zakończono z kodem: $LASTEXITCODE"
    }
}

# ═══════════════════════════════════════════════════════════════
#  3. TESTY JEDNOSTKOWE (CMake + Google Test)
# ═══════════════════════════════════════════════════════════════
function Invoke-Tests {
    Write-Header "Budowanie i uruchamianie testów jednostkowych"

    $cmake = Get-Command "cmake" -ErrorAction SilentlyContinue
    if (-not $cmake) {
        Write-Err "Nie znaleziono cmake. Zainstaluj CMake (https://cmake.org/)."
        return
    }

    $testProjects = @(
        @{ Dir = "01_OOP\02_unit_tests"; Name = "CalculatorTests" }
    )

    foreach ($tp in $testProjects) {
        $srcDir   = Join-Path $RootDir $tp.Dir
        $buildDir = Join-Path $srcDir "build"

        Write-Header "Testy: $($tp.Name)"

        if (-not (Test-Path $srcDir)) {
            Write-Err "Katalog nie istnieje: $srcDir"
            continue
        }

        # CMake configure
        Write-Info "cmake configure → $buildDir"
        New-Item -ItemType Directory -Path $buildDir -Force | Out-Null
        Push-Location $buildDir
        try {
            $out = & cmake .. -G "MinGW Makefiles" 2>&1
            if ($LASTEXITCODE -ne 0) {
                Write-Err "cmake configure nieudane:`n$out"
                Pop-Location
                continue
            }
            Write-Ok "Configure OK"

            # CMake build
            Write-Info "cmake build"
            $out = & cmake --build . 2>&1
            if ($LASTEXITCODE -ne 0) {
                Write-Err "cmake build nieudane:`n$out"
                Pop-Location
                continue
            }
            Write-Ok "Build OK"

            # CTest
            Write-Info "Uruchamianie testów (ctest)"
            Write-Host "──────────────────────────────────────────────" -ForegroundColor DarkGray
            & ctest --output-on-failure
            Write-Host "──────────────────────────────────────────────" -ForegroundColor DarkGray
            if ($LASTEXITCODE -eq 0) {
                Write-Ok "Wszystkie testy przeszły"
            } else {
                Write-Err "Niektóre testy nie przeszły (kod: $LASTEXITCODE)"
            }
        } finally {
            Pop-Location
        }
    }
}

# ═══════════════════════════════════════════════════════════════
#  MAIN
# ═══════════════════════════════════════════════════════════════
switch ($Task) {
    "diagrams" { Invoke-Diagrams }
    "programs" { Invoke-Programs }
    "tests"    { Invoke-Tests    }
    "all" {
        Invoke-Diagrams
        Invoke-Programs
        Invoke-Tests
    }
}

Write-Host "`nGotowe." -ForegroundColor Cyan
